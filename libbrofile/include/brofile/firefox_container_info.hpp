/*
 * Copyright (c) 2024 Oleksandr Porubaimikh
 * SPDX-License-Identifier: MIT
 *
 * See the LICENSE file in the root of this project for details.
 */

#ifndef BROFILE_FIREFOX_CONTAINER_INFO_HPP
#define BROFILE_FIREFOX_CONTAINER_INFO_HPP

#include <string>

namespace bf {
  /**
   * @brief The Firefox container information for a browser.
   */
  struct firefox_container_info {
    std::string name;  /**< The name of the container. */
    std::string color; /**< The color to the container. */
    std::string icon;  /**< The icon to the container. */
  };
}  // namespace bf

#endif  // BROFILE_FIREFOX_CONTAINER_INFO_HPP
