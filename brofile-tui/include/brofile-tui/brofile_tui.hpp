/*
 * Copyright (c) 2024 Oleksandr Porubaimikh
 * SPDX-License-Identifier: MIT
 *
 * See the LICENSE file in the root of this project for details.
 */

#ifndef BROFILE_BROFILE_TUI_HPP
#define BROFILE_BROFILE_TUI_HPP

#include <spdlog/logger.h>

#include <memory>

#include "app_context/app_context.hpp"
#include "brofile/browser_base.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

namespace bftui {
  /**
   * @brief The brofile TUI.
   */
  class brofile_tui {
    std::string url;
    bf::ctx::app_context &context;
    std::vector<std::unique_ptr<bf::browser_base>> browsers;
    ftxui::ScreenInteractive screen;

    std::vector<std::string> browsers_select_items;
    std::vector<std::string> profiles_select_items;
    std::vector<std::string> firefox_containers_select_items;

    std::shared_ptr<ftxui::ComponentBase> open_button;
    std::shared_ptr<ftxui::ComponentBase> url_input;
    std::shared_ptr<ftxui::ComponentBase> browsers_dropdown;
    std::shared_ptr<ftxui::ComponentBase> profiles_dropdown;
    std::shared_ptr<ftxui::ComponentBase> firefox_containers_dropdown;

    bool show_profiles;
    bool show_firefox_containers;

    std::shared_ptr<spdlog::logger> logger;

   public:
    brofile_tui(int argc, char **argv);

   private:
    /**
     * @brief Initializes the browsers.
     */
    void init_browsers();

    /**
     * @brief Initializes the TUI.
     */
    void init_tui();

    /**
     * @brief Initializes the header.
     * @return The header.
     */
    std::shared_ptr<ftxui::ComponentBase> init_header();

    /**
     * @brief Initializes the toolbar.
     * @return The toolbar.
     */
    std::shared_ptr<ftxui::ComponentBase> init_toolbar();

    /**
     * @brief Initializes the browsers menu.
     * @return The browsers menu.
     */
    std::shared_ptr<ftxui::ComponentBase> init_browsers_menu();

    /**
     * @brief Initializes the buttons.
     * @return The buttons.
     */
    std::shared_ptr<ftxui::ComponentBase> init_buttons();

    /**
     * @brief Handles browser selection change.
     * @param reset - reset selections. default is true.
     */
    void on_browser_change(bool reset = true);

    /**
     * @brief Handles profile selection change.
     * @param reset - reset selections. default is true.
     */
    void on_profile_change(bool reset = true);

    /**
     * @brief Sets up required spdlog loggers
     */
    void setup_loggers();
  };
}  // namespace bftui

#endif  // BROFILE_BROFILE_TUI_HPP
