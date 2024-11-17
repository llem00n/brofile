#include "brofile_private/browsers/firefox.hpp"

#include <sys/wait.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "brofile_private/tools/ini_tools.hpp"

bf::firefox::firefox(const std::string &executable,
                     const std::string &config_dir)
    : browser_base(executable),
      config_dir(config_dir),
      incognito(false),
      new_window(false) {}

bool bf::firefox::profiles_available() const { return true; }

std::vector<bf::browser_profile_info> bf::firefox::get_profiles() const {
  std::vector<bf::browser_profile_info> profiles;
  std::filesystem::path config_path =
      std::filesystem::path(std::getenv("HOME")) / config_dir;

  auto profiles_config =
      tools::parse_simple_ini(std::ifstream(config_path / "profiles.ini"));

  for (auto &[_, profile] : profiles_config) {
    if (profile.count("Name")) {
      profiles.push_back({.name = profile["Name"], .path = ""});
    }
  }

  return profiles;
}

void bf::firefox::set_profile(const bf::browser_profile_info &profile) {
  this->profile = profile.name;
}

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
  } else {
    waitpid(pid, nullptr, 0);
    return false;
  }

  return true;
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

std::string bf::firefox::get_browser_name() const { return "Firefox"; }