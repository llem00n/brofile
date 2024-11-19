#ifndef BROFILE_FIREFOX_HPP
#define BROFILE_FIREFOX_HPP

#include <memory>
#include <vector>

#include "brofile/browser_profile_info.hpp"
#include "brofile/firefox_container_info.hpp"

namespace bf {
  /**
   * @brief Firefox browser profile info
   */
  struct firefox_profile_info : public browser_profile_info {
    bool containers_available;                                       /**< Firefox containers enabled flag */
    std::vector<std::unique_ptr<firefox_container_info>> containers; /**< Firefox containers collection */

    firefox_profile_info() = default;
    firefox_profile_info(const firefox_profile_info &) = delete;
    firefox_profile_info(firefox_profile_info &&) = delete;
  };
}  // namespace bf

#endif