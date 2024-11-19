/*
 * Copyright (c) 2024 Oleksandr Porubaimikh
 * SPDX-License-Identifier: MIT
 *
 * See the LICENSE file in the root of this project for details.
 */

#ifndef BROFILE_TOOLS_INI_TOOLS_HPP
#define BROFILE_TOOLS_INI_TOOLS_HPP

#include <fstream>
#include <map>
#include <string>

namespace bf {
  namespace tools {
    std::map<std::string, std::map<std::string, std::string>> parse_simple_ini(
        std::ifstream &file);

    std::map<std::string, std::map<std::string, std::string>> parse_simple_ini(
        std::ifstream &&file);
  }
}  // namespace bf

#endif  // BROFILE_TOOLS_INI_TOOLS_HPP