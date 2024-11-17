#include "app_context/app_context.hpp"

#include <fstream>

using namespace bf::ctx;

std::unique_ptr<app_context> app_context::instance = nullptr;

app_context::app_context() {
  context.new_window = false;
  context.incognito = false;
  context.selected_browser = 0;
  context.selected_profile = 0;
}

app_context& app_context::get_instance() {
  if (instance == nullptr) {
    instance.reset(new app_context());
  }

  return *instance;
}

void app_context::init() {
  auto config_dir = get_config_dir();
  if (!std::filesystem::exists(config_dir / "brofile")) {
    std::filesystem::create_directories(config_dir / "brofile");
  }

  if (!std::filesystem::exists(config_dir / "brofile/cache")) {
    return;
  }

  std::ifstream cache_file(config_dir / "brofile/cache", std::ios::binary);
  cache_file.read(reinterpret_cast<char*>(&context), sizeof(context));
}

void app_context::save() {
  auto config_dir = get_config_dir();
  if (!std::filesystem::exists(config_dir / "brofile")) {
    std::filesystem::create_directory(config_dir / "brofile");
  }

  std::ofstream cache_file(config_dir / "brofile/cache", std::ios::binary);
  cache_file.write(reinterpret_cast<char*>(&context), sizeof(context));
}

_ctx::ctx& app_context::get_context() { return context; }

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