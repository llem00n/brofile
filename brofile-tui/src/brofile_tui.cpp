/*
 * Copyright (c) 2024 Oleksandr Porubaimikh
 * SPDX-License-Identifier: MIT
 *
 * See the LICENSE file in the root of this project for details.
 */

#include "brofile-tui/brofile_tui.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/string.hpp>
#include <iostream>

#include "brofile/browser_factory.hpp"
#include "brofile/firefox.hpp"
#include "brofile/scanner.hpp"

using namespace bftui;

brofile_tui::brofile_tui(int argc, char **argv)
    : screen(ftxui::ScreenInteractive::Fullscreen()), context(bf::ctx::app_context::get_instance()) {
  bf::ctx::app_context::get_instance().init();
  init_browsers();

  if (argc > 1) {
    url = argv[1];
  }

  init_tui();
}

void brofile_tui::init_browsers() {
  browsers.clear();
  auto browser_paths = bf::scanner::scan_default();
  for (auto &path : browser_paths) {
    auto browser = bf::browser_factory::create(path);
    if (browser) {
      browsers.push_back(std::move(browser));
    }
  }
}

void brofile_tui::init_tui() {
  using namespace ftxui;

  auto header = init_header();
  auto toolbar = init_toolbar();
  auto browsers_menu = init_browsers_menu();
  auto buttons = init_buttons();

  auto container =
      Container::Vertical({
          header,
          toolbar,
          browsers_menu | flex,
          buttons,
      }) |
      CatchEvent([&](const Event &e) {
        if (!url_input->Focused() && (e == Event::Character('c') || e == Event::Character('C'))) {
          screen.ExitLoopClosure()();
          return true;
        }

        if (!url_input->Focused() && (e == Event::Character('n') || e == Event::Character('N'))) {
          context.new_window = !context.new_window;
          return true;
        }

        if (!url_input->Focused() && (e == Event::Character('i') || e == Event::Character('I'))) {
          context.incognito = !context.incognito;
          return true;
        }

        if (!url_input->Focused() && (e == Event::Character('u') || e == Event::Character('U'))) {
          url_input->TakeFocus();
          return true;
        }

        if (!url_input->Focused() && (e == Event::Character('o') || e == Event::Character('O'))) {
          open_button->OnEvent(Event::Return);
          return true;
        }

        if (!url_input->Focused() && (e == Event::Character('b') || e == Event::Character('B'))) {
          browsers_dropdown->ChildAt(0)->ChildAt(0)->OnEvent(Event::Return);
          return true;
        }

        if (show_profiles && !url_input->Focused() && (e == Event::Character('p') || e == Event::Character('P'))) {
          profiles_dropdown->ChildAt(0)->ChildAt(0)->OnEvent(Event::Return);
          return true;
        }

        if (show_firefox_containers && !url_input->Focused() &&
            (e == Event::Character('f') || e == Event::Character('F'))) {
          firefox_containers_dropdown->ChildAt(0)->ChildAt(0)->OnEvent(Event::Return);
          return true;
        }

        return false;
      });

  screen.Loop(container);
}

std::shared_ptr<ftxui::ComponentBase> brofile_tui::init_header() {
  using namespace ftxui;
  return Renderer([&] { return vbox(text(L"brofile") | center, separator()); });
}

std::shared_ptr<ftxui::ComponentBase> brofile_tui::init_toolbar() {
  using namespace ftxui;

  url_input = Input(&url, L"<empty>");

  return Container::Vertical({
      Container::Horizontal({
          Renderer([&] { return text(L"  [u]rl: "); }),
          url_input,
      }),
      Checkbox(L"[n]ew window", &context.new_window),
      Checkbox(L"[i]ncognito", &context.incognito),
      Renderer([&] { return separator(); }),
  });
}

std::shared_ptr<ftxui::ComponentBase> brofile_tui::init_browsers_menu() {
  using namespace ftxui;

  show_profiles = false;

  browsers_select_items.clear();
  profiles_select_items.clear();
  firefox_containers_select_items.clear();

  browsers_select_items.emplace_back("<none>");
  std::transform(browsers.begin(), browsers.end(), std::back_inserter(browsers_select_items),
                 [](auto &browser) { return browser->get_browser_name(); });

  if (context.selected_browser > browsers.size()) {
    context.selected_browser = 0;
  }

  browsers_dropdown = Dropdown({.radiobox = {.entries = &browsers_select_items,
                                             .selected = &context.selected_browser,
                                             .on_change = [&] { on_browser_change(); }}});
  on_browser_change(false);

  profiles_dropdown = Dropdown({.radiobox = {.entries = &profiles_select_items,
                                             .selected = &context.selected_profile,
                                             .on_change = [&] { on_profile_change(); }}});
  on_profile_change(false);

  firefox_containers_dropdown = Dropdown({.radiobox = {.entries = &firefox_containers_select_items,
                                                       .selected = &context.selected_firefox_container,
                                                       .on_change = [&] {}}});

  auto container = Container::Horizontal(
      {Container::Vertical({Renderer([] { return text("[b]rowser"); }), browsers_dropdown}),
       Container::Vertical({Renderer([] { return text("[p]rofile"); }), profiles_dropdown}) | Maybe(&show_profiles),
       Container::Vertical({Renderer([] { return text("[f]irefox container"); }), firefox_containers_dropdown}) |
           Maybe(&show_firefox_containers)});

  return container;
}

std::shared_ptr<ftxui::ComponentBase> brofile_tui::init_buttons() {
  using namespace ftxui;

  open_button =
      Button(
          "[o]pen",
          [&] {
            auto &browser = browsers[context.selected_browser - 1];
            if (context.selected_profile >= 1 && context.selected_profile <= browser->get_profiles().size()) {
              auto &profile = *browser->get_profiles()[context.selected_profile - 1];
              browser->set_profile(profile);

              if (browser->get_type() == bf::browser_type::FIREFOX) {
                auto &firefox_profile = static_cast<bf::firefox_profile_info &>(profile);
                if (context.selected_firefox_container >= 1 &&
                    context.selected_firefox_container <= firefox_profile.containers.size()) {
                  browser->set_firefox_container(*firefox_profile.containers[context.selected_firefox_container - 1]);
                }
              }
            }

            browser->set_url(url);
            browser->set_new_window(context.new_window);
            browser->set_incognito(context.incognito);

            bf::ctx::app_context::get_instance().save();

            browser->open();

            screen.ExitLoopClosure()();
          },
          ButtonOption::Ascii()) |
      color(Color::Green);

  return Container::Horizontal({
             Button(
                 "[c]ancel", [&] { screen.ExitLoopClosure()(); }, ButtonOption::Ascii()) |
                 color(Color::Red),
             open_button,
         }) |
         align_right;
}

void brofile_tui::on_browser_change(bool reset) {
  show_profiles = false;
  show_firefox_containers = false;

  if (reset) {
    context.selected_profile = 0;
    context.selected_firefox_container = 0;
  }

  if (!context.selected_browser) return;
  auto &browser = browsers[context.selected_browser - 1];
  if (browser->profiles_available()) {
    profiles_select_items.clear();
    profiles_select_items.emplace_back("<none>");
    std::transform(browser->get_profiles().begin(), browser->get_profiles().end(),
                   std::back_inserter(profiles_select_items), [](auto &profile) { return profile->name; });
    show_profiles = true;
  }
}

void brofile_tui::on_profile_change(bool reset) {
  show_firefox_containers = false;

  if (reset) {
    context.selected_firefox_container = 0;
  }

  if (!context.selected_browser || !context.selected_profile) return;
  auto &browser = browsers[context.selected_browser - 1];
  auto &profile = *browser->get_profiles()[context.selected_profile - 1];

  if (browser->get_type() == bf::browser_type::FIREFOX) {
    auto &firefox_profile = static_cast<bf::firefox_profile_info &>(profile);
    if (firefox_profile.containers_available) {
      firefox_containers_select_items.clear();
      firefox_containers_select_items.emplace_back("<none>");
      std::transform(firefox_profile.containers.begin(), firefox_profile.containers.end(),
                     std::back_inserter(firefox_containers_select_items),
                     [](auto &container) { return container->name; });
      show_firefox_containers = true;
    }
  }
}