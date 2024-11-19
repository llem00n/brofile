/*
 * Copyright (c) 2024 Oleksandr Porubaimikh
 * SPDX-License-Identifier: MIT
 *
 * See the LICENSE file in the root of this project for details.
 */

#ifndef BROFILE_LOGGING_HPP
#define BROFILE_LOGGING_HPP

#include <string>
#include <memory>
#include <spdlog/spdlog.h>

namespace bf {
  /**
   * @brief configures logging with spdlog
   * @param name - logger name
   * @param log_level - log level. must be one of the following: "debug" | "info" | "warn" | "error" | "critical"
   * @returns app's spdlog logger
   */
  std::shared_ptr<spdlog::logger> setup_logger(const std::string &name, const std::string &log_level);
}  // namespace bf

#endif