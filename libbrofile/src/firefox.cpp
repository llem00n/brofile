/*
 * Copyright (c) 2024 Oleksandr Porubaimikh
 * SPDX-License-Identifier: MIT
 *
 * See the LICENSE file in the root of this project for details.
 */

#include "brofile_private/browsers/firefox.hpp"

#include <sys/wait.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "brofile_private/tools/ini_tools.hpp"

bf::firefox::firefox(const std::string &executable, const std::string &config_dir)
    : browser_base(executable, browser_type::FIREFOX), config_dir(config_dir), incognito(false), new_window(false) {
  scan_profiles();
}

bool bf::firefox::profiles_available() const { return true; }

void bf::firefox::scan_profiles() {
  profiles.clear();

  std::filesystem::path config_path = std::filesystem::path(std::getenv("HOME")) / config_dir;

  auto profiles_config = tools::parse_simple_ini(std::ifstream(config_path / "profiles.ini"));

  for (auto &[_, profile] : profiles_config) {
    if (profile.count("Name")) {
      auto profile_ptr = std::make_unique<firefox_profile_info>();
      profile_ptr->name = profile["Name"];
      profile_ptr->path = profile["Path"];
      scan_firefox_containers(*profile_ptr);
      profiles.push_back(std::move(profile_ptr));
    }
  }
}

const std::vector<std::unique_ptr<bf::browser_profile_info>> &bf::firefox::get_profiles() const {
  return profiles;
  }

void bf::firefox::scan_firefox_containers(firefox_profile_info &profile) {
  profile.containers.clear();

  std::filesystem::path profile_config_path = std::filesystem::path(std::getenv("HOME")) / config_dir / profile.path;

  // are containers enabled?
  bool containers_enabled = false;
  if (!std::filesystem::exists(profile_config_path / "prefs.js")) {
    throw std::runtime_error("prefs.js file is missing");
  }

  std::ifstream prefsjs(profile_config_path / "prefs.js");
  if (!prefsjs.is_open()) {
    throw std::runtime_error("failed to open prefs.js");
  }

  std::string line;
  while (std::getline(prefsjs, line)) {
    if (line.find("user_pref(\"privacy.userContext.enabled\", true);") != std::string::npos) {
      containers_enabled = true;
    }
  }

  // is "Open external links in a container" addon installed?
  bool addon_installed = false;
  if (!std::filesystem::exists(profile_config_path / "extensions.json")) {
    throw std::runtime_error("extensions.json is missing");
  }
  auto addons = nlohmann::json::parse(std::ifstream(profile_config_path / "extensions.json"));
  for (auto &addon : addons["addons"]) {
    if (addon["defaultLocale"]["name"] == "Open external links in a container") addon_installed = true;
  }

  profile.containers_available = containers_enabled && addon_installed;
  if (!profile.containers_available) return;

  if (!std::filesystem::exists(profile_config_path / "containers.json")) {
    throw std::runtime_error("containers.json file is missing");
  }
  auto containers_json = nlohmann::json::parse(std::ifstream(profile_config_path / "containers.json"));
  if (!containers_json["identities"].is_array()) {
    throw std::runtime_error("containers.json format is unknown");
  }

  auto x = containers_json["identities"].size();

  for (const auto &container_json : containers_json["identities"]) {
    if (container_json["public"] != true) {
      std::string name = container_json["name"];
      continue;
    }

    auto container = std::make_unique<firefox_container_info>();

    if (container_json.contains("l10nId")) {
      std::map<std::string, std::string> locale_mapping = {
          {"user-context-personal", "Personal"},
          {"user-context-work", "Work"},
          {"user-context-banking", "Banking"},
          {"user-context-shopping", "Shopping"},
      };
      container->name = locale_mapping.find(container_json["l10nId"]) != locale_mapping.end()
                                 ? locale_mapping[container_json["l10nId"]]
                                 : container_json.value("l10nId", "");
    } else {
      container->name = container_json.value("name", "<bad name>");
    }

    container->icon = container_json["icon"];
    container->color = container_json["color"];
    profile.containers.push_back(std::move(container));
  }
}

void bf::firefox::set_profile(const bf::browser_profile_info &profile) { this->profile = profile.name; }

void bf::firefox::set_firefox_container(const firefox_container_info &container) { this->container = container; }

bool bf::firefox::set_url(const std::string &url) {
  this->url = url;
  return true;
}

bool bf::firefox::set_new_window(bool new_window) {
  this->new_window = new_window;
  return true;
}

bool bf::firefox::set_incognito(bool incognito) {
  this->incognito = incognito;
  return true;
}

bool bf::firefox::open() {
  pid_t pid = fork();
  if (pid == 0) {
    _open();
    return true;
  } else {
    return false;
  }
}

void bf::firefox::_open() const {
  std::string executable = get_executable();
  std::vector<std::string> args = {executable};

  if (!profile.empty()) {
    args.push_back("-P");
    args.push_back(profile);
  }

  if (new_window) {
    args.push_back("--new-window");
  }

  if (incognito) {
    args.push_back("--private-window");
  }

  std::string full_url;
  if (!container.name.empty()) {
    full_url = "ext+container:name=" + container.name + "&url=", container.name;
  }

  full_url += !url.empty() ? url : "about:blank";

  args.push_back(full_url);

  char **argv = new char *[args.size() + 1];
  for (size_t i = 0; i < args.size(); i++) {
    argv[i] = new char[args[i].size() + 1];
    std::strcpy(argv[i], args[i].c_str());
  }
  argv[args.size()] = nullptr;

  execvp(executable.c_str(), argv);
}

std::string bf::firefox::get_browser_name() const { return "Firefox"; }