#ifndef BROFILE_FIREFOX_PRIVATE_HPP
#define BROFILE_FIREFOX_PRIVATE_HPP

#include <filesystem>
#include <map>
#include <string>

#include "brofile/browser_base.hpp"
#include "brofile/firefox.hpp"

namespace bf {
  /**
   * @brief The Firefox browser.
   */
  class firefox : public browser_base {
    std::string config_dir;
    std::string url;
    std::string profile;
    firefox_container_info container;
    bool new_window;
    bool incognito;
    std::vector<std::unique_ptr<browser_profile_info>> profiles;

   public:
    explicit firefox(const std::string &executable, const std::string &config_dir = ".mozilla/firefox");
    ~firefox() override = default;

    /**
     * @brief Checks if profiles are available.
     * @return true if profiles are available, false otherwise.
     */
    bool profiles_available() const override;

    /**
     * @brief Gets the profiles.
     * @return A vector of profiles.
     * @throws std::runtime_error if profiles are not available.
     */
    const std::vector<std::unique_ptr<browser_profile_info>> &get_profiles() const override;

    /**
     * @brief Sets the profile.
     * @param profile The profile to set.
     * @throws std::runtime_error if profiles are not available.
     */
    void set_profile(const browser_profile_info &profile) override;

    /**
     * @brief Sets the Firefox container.
     * @param profile The Firefox container to set.
     * @throws std::runtime_error if Firefox containers are not available.
     */
    void set_firefox_container(const firefox_container_info &container) override;

    /**
     * @brief Sets the URL.
     * @param url The URL to set.
     * @return true if the URL was set, false otherwise.
     */
    bool set_url(const std::string &url) override;

    /**
     * @brief Sets if a new window should be opened.
     * @param new_window true if a new window should be opened, false otherwise.
     * @return true if the new window flag was set, false otherwise.
     */
    bool set_new_window(bool new_window) override;

    /**
     * @brief Sets if incognito mode should be used.
     * @param incognito true if incognito mode should be used, false otherwise.
     * @return true if incognito mode was set, false otherwise.
     */
    bool set_incognito(bool incognito) override;

    /**
     * @brief Opens the browser.
     * @return true if the browser was opened, false otherwise.
     */
    bool open() override;

    /**
     * @brief Returns the browser name.
     * @return The browser name.
     */
    virtual std::string get_browser_name() const override;

   private:
    /**
     * @brief Starts the browser process using execv.
     */
    void _open() const;

    /**
     * @brief Scans for profiles
     */
    void scan_profiles();

    /**
     * @brief Scans for Firefox containers
     */
    void scan_firefox_containers(firefox_profile_info &profile);
  };
}  // namespace bf

#endif  // BROFILE_FIREFOX_PRIVATE_HPP
