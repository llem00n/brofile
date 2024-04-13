#include "brofile_private/browsers/chromium.hpp"
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

bf::chromium::chromium(const std::string &executable, const std::string &config_dir): browser_base(executable), config_dir(config_dir) {}

bool bf::chromium::profiles_available() const {
  return true;
}

std::vector<bf::browser_profile_info> bf::chromium::get_profiles() const {
  std::vector<bf::browser_profile_info> profiles;
  std::filesystem::path path = std::filesystem::path(std::getenv("HOME")) / ".config" / config_dir;

  auto local_state = nlohmann::json::parse(std::ifstream(path / "Local State"));
  for (auto &[key, profile]: local_state["profile"]["info_cache"].items()) {
    profiles.push_back({
      .name = profile["name"],
      .path = key
    });
  }

  return profiles;
}

void bf::chromium::set_profile(const bf::browser_profile_info &profile) {
  this->profile = profile.path;
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
  } else {
    waitpid(pid, nullptr, 0);
    return false;
  }

  return true;
}

void bf::chromium::_open() const {
  std::string executable = get_executable();
  std::vector<std::string> args = { executable };

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

  char **argv = new char*[args.size() + 1];
  for (size_t i = 0; i < args.size(); i++) {
    argv[i] = new char[args[i].size() + 1];
    std::strcpy(argv[i], args[i].c_str());
  }
  argv[args.size()] = nullptr;

  execv(executable.c_str(), argv);
}

std::string bf::chromium::get_browser_name() const {
  if (config_dir == "google-chrome") {
    return "Google Chrome";
  } else {
    return "Chromium";
  }
}