#ifndef BROFILE_BROWSER_BASE_HPP
#define BROFILE_BROWSER_BASE_HPP

#include <string>
#include <vector>

#include "brofile/browser_profile_info.hpp"

namespace bf {
  /**
   * @brief The base class for browsers.
   */
  class browser_base {
    std::string executable;

   public:
    explicit browser_base(const std::string& executable);
    virtual ~browser_base() = default;

    const std::string& get_executable() const;

    /**
     * @brief Checks if profiles are available.
     * @return true if profiles are available, false otherwise.
     */
    virtual bool profiles_available() const = 0;

    /**
     * @brief Gets the profiles.
     * @return A vector of profiles.
     * @throws std::runtime_error if profiles are not available.
     */
    virtual std::vector<browser_profile_info> get_profiles() const = 0;

    /**
     * @brief Sets the profile.
     * @param profile The profile to set.
     * @throws std::runtime_error if profiles are not available.
     */
    virtual void set_profile(const browser_profile_info& profile) = 0;

    /**
     * @brief Sets the URL.
     * @param url The URL to set.
     * @return true if the URL was set, false otherwise.
     */
    virtual bool set_url(const std::string& url) = 0;

    /**
     * @brief Sets if a new window should be opened.
     * @param new_window true if a new window should be opened, false otherwise.
     * @return true if the new window flag was set, false otherwise.
     */
    virtual bool set_new_window(bool new_window) = 0;

    /**
     * @brief Sets if incognito mode should be used.
     * @param incognito true if incognito mode should be used, false otherwise.
     * @return true if incognito mode was set, false otherwise.
     */
    virtual bool set_incognito(bool incognito) = 0;

    /**
     * @brief Opens the browser.
     * @return true if the browser was opened, false otherwise.
     */
    virtual bool open() = 0;

    /**
     * @brief Returns the browser name.
     * @return The browser name.
     */
    virtual std::string get_browser_name() const = 0;
  };
}  // namespace bf

#endif  // BROFILE_BROWSER_BASE_HPP
