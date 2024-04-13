#include "brofile-tui/brofile_tui.hpp"
#include "brofile/scanner.hpp"
#include "brofile/browser_factory.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/screen/string.hpp>
#include "ftxui/component/screen_interactive.hpp"

using namespace bftui;

brofile_tui::brofile_tui(int argc, char **argv): new_window(false), incognito(false), selected_browser(0), selected_profile(0), screen(ftxui::ScreenInteractive::Fullscreen()) {
  init_browsers();

  if (argc > 1) {
    url = argv[1];
  }

  init_tui();
}

void brofile_tui::init_browsers() {
  browsers.clear();
  auto browser_paths = bf::scanner::scan_default();
  for (auto &path: browser_paths) {
    auto browser = bf::browser_factory::create(path);
    if (browser) {
      auto profiles = browser->profiles_available() ? browser->get_profiles() : std::vector<bf::browser_profile_info>();
      browsers.push_back({
        .browser = std::move(browser),
        .profiles = std::move(profiles)
      });
    }
  }
}

void brofile_tui::init_tui() {
  using namespace ftxui;

  auto header = init_header();
  auto toolbar = init_toolbar();
  auto browsers_menu = init_browsers_menu();
  auto buttons = init_buttons();

  auto container = Container::Vertical({
    header,
    toolbar,
    browsers_menu | flex,
    buttons,
  }) | CatchEvent([&] (const Event &e) {
    if (!url_input->Focused() && (e == Event::Character('c') || e == Event::Character('C'))) {
      screen.ExitLoopClosure()();
      return true;
    }

    if (!url_input->Focused() && (e == Event::Character('n') || e == Event::Character('N'))) {
      new_window = !new_window;
      return true;
    }

    if (!url_input->Focused() && (e == Event::Character('i') || e == Event::Character('I'))) {
      incognito = !incognito;
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

    return false;
  });

  screen.Loop(container);
}

std::shared_ptr<ftxui::ComponentBase> brofile_tui::init_header() {
  using namespace ftxui;
  return Renderer([&] {
    return vbox(text(L"Brofile TUI") | color(Color::Yellow) | bold, separator());
  });
}

std::shared_ptr<ftxui::ComponentBase> brofile_tui::init_toolbar() {
  using namespace ftxui;

  url_input = Input(&url, L"<empty>");

  return Container::Vertical({
    Container::Horizontal({
      Renderer([&] { return text(L"[u] URL: "); }),
      url_input,
    }),
    Checkbox(L"[n] New window", &new_window),
    Checkbox(L"[i] Incognito", &incognito),
    Renderer([&] { return separator(); }),
  });
}

std::shared_ptr<ftxui::ComponentBase> brofile_tui::init_browsers_menu() {
  using namespace ftxui;

  left_menu_items.clear();
  std::transform(browsers.begin(), browsers.end(), std::back_inserter(left_menu_items), [] (auto &browser) {
    return browser.browser->get_browser_name();
  });

  right_menu_items.clear();
  if (selected_browser < browsers.size()) {
    auto &browser = browsers[selected_browser];
    right_menu_items.emplace_back("<None>");
    std::transform(browser.profiles.begin(), browser.profiles.end(), std::back_inserter(right_menu_items), [] (auto &profile) {
      return profile.name;
    });
  }

  auto left_menu_options = MenuOption();
  left_menu_options.on_change = [&] {
    selected_profile = 0;
    right_menu_items.clear();
    if (selected_browser < browsers.size()) {
      auto &browser = browsers[selected_browser];
      right_menu_items.emplace_back("<None>");
      std::transform(browser.profiles.begin(), browser.profiles.end(), std::back_inserter(right_menu_items),
                     [](auto &profile) {
                       return profile.name;
                     });
    }
  };

  left_menu = Menu(&left_menu_items, &selected_browser, left_menu_options);
  right_menu = Menu(&right_menu_items, &selected_profile);

  return Container::Horizontal({
    left_menu | flex | border,
    right_menu | flex | border,
  });
}

std::shared_ptr<ftxui::ComponentBase> brofile_tui::init_buttons() {
  using namespace ftxui;

  open_button = Button("[o] Open", [&] {
    auto &browser = browsers[selected_browser];
    if (selected_profile > 1 && selected_profile <= browser.profiles.size()) {
      browser.browser->set_profile(browser.profiles[selected_profile - 1]);
    }

    browser.browser->set_url(url);

    if (new_window) {
      browser.browser->set_new_window(true);
    }

    if (incognito) {
      browser.browser->set_incognito(true);
    }

    browser.browser->open();

    screen.ExitLoopClosure()();
  }, ButtonOption::Border()) | color(Color::Green);

  return Container::Horizontal({
    Button("[c] Cancel", [&] {
      screen.ExitLoopClosure()();
    }, ButtonOption::Border()) | color(Color::Red),
    open_button,
  }) | align_right;
}