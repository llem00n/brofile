#ifndef BROFILE_SCANNER_HPP
#define BROFILE_SCANNER_HPP

#include <vector>
#include <filesystem>

namespace bf {
  /**
   * @brief Scans $XDG_DATA_HOME and $XDG_DATA_DIRS directories for browser .desktop files.
   */
  class scanner {
  public:
    scanner() = delete;
    scanner(const scanner&) = delete;
    scanner(scanner&&) = delete;
    scanner& operator=(const scanner&) = delete;
    scanner& operator=(scanner&&) = delete;

    /**
     * @brief Scans $XDG_DATA_HOME and $XDG_DATA_DIRS directories for browser .desktop files.
     * @return A vector of paths to files.
     */
    static std::vector<std::filesystem::path> scan_default();

    /**
     * @brief Scans a directory for browser .desktop files.
     * @param dir The directory to scan_default.
     * @return A vector of paths to files.
     */
    static std::vector<std::filesystem::path> scan(const std::filesystem::path& dir);

  private:
    /**
     * @brief Checks if a .desktop file is a browser.
     * @param file The path to the .desktop file.
     * @return true if the file is a browser, false otherwise.
     */
    static bool is_browser(const std::filesystem::path& file);

    /**
     * @brief Splits an environment variable by ':' into a vector of paths.
     * @param env The environment variable to split.
     * @return A vector of paths.
     */
    static std::vector<std::string> split_env(const std::string& env);
  };
}

#endif //BROFILE_SCANNER_HPP
