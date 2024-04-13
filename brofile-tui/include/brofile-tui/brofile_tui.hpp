#ifndef BROFILE_BROFILE_TUI_HPP
#define BROFILE_BROFILE_TUI_HPP

#include "brofile/browser_base.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <memory>

namespace bftui {
  /**
   * @brief The browser information.
   */
  struct browser_info {
    std::unique_ptr<bf::browser_base> browser;
    std::vector<bf::browser_profile_info> profiles;
  };

  /**
   * @brief The brofile TUI.
   */
  class brofile_tui {
    std::vector<browser_info> browsers;
    std::string url;
    bool new_window;
    bool incognito;
    std::shared_ptr<ftxui::ComponentBase> url_input;
    ftxui::ScreenInteractive screen;
    std::vector<std::string> left_menu_items;
    std::shared_ptr<ftxui::ComponentBase> left_menu;
    int selected_browser;
    std::vector<std::string> right_menu_items;
    std::shared_ptr<ftxui::ComponentBase> right_menu;
    int selected_profile;
    std::shared_ptr<ftxui::ComponentBase> open_button;

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
  };
}

#endif //BROFILE_BROFILE_TUI_HPP
