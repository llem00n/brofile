/*
 * Copyright (c) 2024 Oleksandr Porubaimikh
 * SPDX-License-Identifier: MIT
 *
 * See the LICENSE file in the root of this project for details.
 */

#include <spdlog/spdlog.h>

#include <csignal>

#include "brofile-tui/brofile_tui.hpp"

void sig_handler(int signal) { spdlog::shutdown(); }

int main(int argc, char **argv) {
  std::signal(SIGSEGV, &sig_handler);
  std::signal(SIGABRT, &sig_handler);
  std::signal(SIGINT, &sig_handler);

  bftui::brofile_tui tui(argc, argv);

  spdlog::shutdown();
  return 0;
}
