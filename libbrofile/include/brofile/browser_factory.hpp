#ifndef BROFILE_BROWSER_FACTORY_HPP
#define BROFILE_BROWSER_FACTORY_HPP

#include <memory>
#include <filesystem>
#include "brofile/browser_base.hpp"
#include "brofile/browser_type.hpp"

namespace bf {
  /**
   * @brief A factory for creating browsers.
   */
  class browser_factory {
  public:
    browser_factory() = delete;
    browser_factory(const browser_factory&) = delete;
    browser_factory(browser_factory&&) = delete;
    browser_factory& operator=(const browser_factory&) = delete;
    browser_factory& operator=(browser_factory&&) = delete;

    /**
     * @brief Creates a browser launcher instance.
     * @param file The path to the browser .desktop file.
     * @return A unique pointer to the browser launcher.
     */
    static std::unique_ptr<browser_base> create(const std::filesystem::path& file);

  private:
    /**
     * @brief Gets the type of browser.
     * @param file The path to the browser .desktop file.
     * @return The type of browser.
     */
    static browser_type get_browser_type(const std::filesystem::path& file);

    /**
     * @brief Gets the executable of the browser.
     * @param file The path to the browser .desktop file.
     * @return The executable of the browser.
     */
    static std::string get_browser_executable(const std::filesystem::path& file);
  };
}

#endif //BROFILE_BROWSER_FACTORY_HPP
