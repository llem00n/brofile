/*
 * Copyright (c) 2024 Oleksandr Porubaimikh
 * SPDX-License-Identifier: MIT
 *
 * See the LICENSE file in the root of this project for details.
 */

#include "brofile/browser_factory.hpp"

#include <fstream>

#include "brofile_private/browsers/chromium.hpp"
#include "brofile_private/browsers/firefox.hpp"

std::unique_ptr<bf::browser_base> bf::browser_factory::create(
    const std::filesystem::path &file) {
  auto type = get_browser_type(file);
  auto executable = get_browser_executable(file);

  switch (type) {
    case bf::browser_type::CHROMIUM:
      return std::make_unique<bf::chromium>(executable);
    case bf::browser_type::FIREFOX:
      return std::make_unique<bf::firefox>(executable);
      return nullptr;
    case bf::browser_type::GOOGLE_CHROME:
      return std::make_unique<bf::chromium>(executable, "google-chrome");
    case bf::browser_type::UNSUPPORTED:
      return nullptr;
  }

  return nullptr;
}

bf::browser_type bf::browser_factory::get_browser_type(
    const std::filesystem::path &file) {
  std::ifstream stream(file);
  for (std::string line; std::getline(stream, line);) {
    if (line.find("Name") != std::string::npos &&
        line.find("Chromium") != std::string::npos) {
      return bf::browser_type::CHROMIUM;
    }

    if (line.find("Name") != std::string::npos &&
        line.find("Firefox") != std::string::npos) {
      return bf::browser_type::FIREFOX;
    }

    if (line.find("Name") != std::string::npos &&
        line.find("Google Chrome") != std::string::npos) {
      return bf::browser_type::GOOGLE_CHROME;
    }
  }

  return bf::browser_type::UNSUPPORTED;
}

std::string bf::browser_factory::get_browser_executable(
    const std::filesystem::path &file) {
  std::ifstream stream(file);
  for (std::string line; std::getline(stream, line);) {
    if (line.find("Exec") != std::string::npos) {
      auto command = line.substr(line.find("=") + 1);
      return command.substr(
          0,
          command.find(
              " "));  // Assume the executable does not have spaces in its path.
    }
  }

  return "";
}