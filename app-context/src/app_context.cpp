/*
 * Copyright (c) 2024 Oleksandr Porubaimikh
 * SPDX-License-Identifier: MIT
 *
 * See the LICENSE file in the root of this project for details.
 */

#include "app_context/app_context.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using namespace bf::ctx;

std::unique_ptr<app_context> app_context::instance = nullptr;

app_context::app_context() {
  new_window = false;
  incognito = false;
  selected_browser = 0;
  selected_profile = 0;
  selected_firefox_container = 0;
}

app_context& app_context::get_instance() {
  if (instance == nullptr) {
    instance.reset(new app_context());
  }

  return *instance;
}

void app_context::init() {
  remove_cache_v1();

  auto config_dir = get_config_dir();
  if (!std::filesystem::exists(config_dir / "brofile")) {
    std::filesystem::create_directories(config_dir / "brofile");
  }

  if (!std::filesystem::exists(config_dir / "brofile/settings.json")) {
    return;
  }

  auto json = nlohmann::json::parse(std::ifstream(config_dir / "brofile/settings.json"));

  new_window = json.value("new_window", false);
  incognito = json.value("incognito", false);
  selected_browser = json.value("selected_browser", 0);
  selected_profile = json.value("selected_profile", 0);
  selected_firefox_container = json.value("selected_firefox_container", 0);
}

void app_context::save() {
  remove_cache_v1();

  auto config_dir = get_config_dir();
  if (!std::filesystem::exists(config_dir / "brofile")) {
    std::filesystem::create_directory(config_dir / "brofile");
  }

  std::ofstream file(config_dir / "brofile/settings.json");
  nlohmann::json json = nlohmann::json::object({
    { "new_window", new_window },
    { "incognito", incognito },
    { "selected_browser", selected_browser },
    { "selected_profile", selected_profile },
    { "selected_firefox_container", selected_firefox_container },
  });
  file << json;
}

std::filesystem::path app_context::get_config_dir() {
  auto xdg_config_home = std::getenv("XDG_CONFIG_HOME");
  if (xdg_config_home != nullptr) {
    return std::filesystem::path(xdg_config_home);
  }

  auto home = std::getenv("HOME");
  if (home != nullptr) {
    return std::filesystem::path(home) / ".config";
  }

  return std::filesystem::path("~/.config");
}

void app_context::remove_cache_v1() {
  auto config_dir = get_config_dir();
  if (std::filesystem::exists(config_dir / "brofile/cache")) {
    std::filesystem::remove(config_dir / "brofile/cache");
  }
}