#ifndef BROFILE_BROWSER_PROFILE_INFO_HPP
#define BROFILE_BROWSER_PROFILE_INFO_HPP

#include <string>

namespace bf {
  /**
   * @brief The profile information for a browser.
   */
  struct browser_profile_info {
    std::string name; /**< The name of the profile. */
    std::string path; /**< The path to the profile. */
  };
}  // namespace bf

#endif  // BROFILE_BROWSER_PROFILE_INFO_HPP
