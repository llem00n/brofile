/*
 * Copyright (c) 2024 Oleksandr Porubaimikh
 * SPDX-License-Identifier: MIT
 *
 * See the LICENSE file in the root of this project for details.
 */

#include "brofile_private/browsers/chromium.hpp"

#include <sys/wait.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

bf::chromium::chromium(const std::string &executable, const std::string &config_dir)
    : browser_base(executable, browser_type::CHROMIUM), config_dir(config_dir), incognito(false), new_window(false) {
  scan_profiles();
}

bool bf::chromium::profiles_available() const { return true; }

void bf::chromium::scan_profiles() {
  std::filesystem::path path = std::filesystem::path(std::getenv("HOME")) / ".config" / config_dir;

  auto local_state = nlohmann::json::parse(std::ifstream(path / "Local State"));
  for (auto &[key, profile] : local_state["profile"]["info_cache"].items()) {
    auto profile_ptr = std::make_unique<browser_profile_info>();
    profile_ptr->name = profile["name"];
    profile_ptr->path = key;
    profiles.push_back(std::move(profile_ptr));
  }
}

const std::vector<std::unique_ptr<bf::browser_profile_info>> &bf::chromium::get_profiles() const { return profiles; }

void bf::chromium::set_profile(const bf::browser_profile_info &profile) { this->profile = profile.path; }

void bf::chromium::set_firefox_container(const firefox_container_info &container) {
  throw std::runtime_error("Chromium does not support Firefox containers");
}

bool bf::chromium::set_url(const std::string &url) {
  this->url = url;
  return true;
}

bool bf::chromium::set_new_window(bool new_window) {
  this->new_window = new_window;
  return true;
}

bool bf::chromium::set_incognito(bool incognito) {
  this->incognito = incognito;
  return true;
}

bool bf::chromium::open() {
  pid_t pid = fork();
  if (pid == 0) {
    _open();
    return true;
  } else {
    return false;
  }
}

void bf::chromium::_open() const {
  std::string executable = get_executable();
  std::vector<std::string> args = {executable};

  if (new_window) {
    args.push_back("--new-window");
  }

  if (incognito) {
    args.push_back("--incognito");
  }

  if (!profile.empty()) {
    args.push_back("--profile-directory=" + profile);
  }

  if (!url.empty()) {
    args.push_back(url);
  }

  char **argv = new char *[args.size() + 1];
  for (size_t i = 0; i < args.size(); i++) {
    argv[i] = new char[args[i].size() + 1];
    std::strcpy(argv[i], args[i].c_str());
  }
  argv[args.size()] = nullptr;

  execvp(executable.c_str(), argv);
}

std::string bf::chromium::get_browser_name() const {
  if (config_dir == "google-chrome") {
    return "Google Chrome";
  } else {
    return "Chromium";
  }
}