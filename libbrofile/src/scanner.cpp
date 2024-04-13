#include <brofile/scanner.hpp>
#include <fstream>

std::vector<std::filesystem::path> bf::scanner::scan_default() {
  auto xdg_data_home_env = std::getenv("XDG_DATA_HOME");
  auto xdg_data_dirs_env = std::getenv("XDG_DATA_DIRS");

  std::vector<std::filesystem::path> files, tmp;

  if (xdg_data_home_env) {
    for (const auto &dir: split_env(xdg_data_home_env)) {
      tmp = scan(std::filesystem::path(dir) / "applications");
      files.insert(files.end(), tmp.begin(), tmp.end());
    }
  }

  if (xdg_data_dirs_env) {
    for (const auto &dir: split_env(xdg_data_dirs_env)) {
      tmp = scan(std::filesystem::path(dir) / "applications");
      files.insert(files.end(), tmp.begin(), tmp.end());
    }
  }

  return files;
}

std::vector<std::filesystem::path> bf::scanner::scan(const std::filesystem::path &dir) {
  std::vector<std::filesystem::path> files;

  if (!std::filesystem::exists(dir)) {
    return files;
  }

  for (const auto &entry: std::filesystem::directory_iterator(dir)) {
    if (entry.path().extension() == ".desktop" && is_browser(entry.path())) {
      files.push_back(entry.path());
    }
  }

  return files;
}

bool bf::scanner::is_browser(const std::filesystem::path &file) {
  std::ifstream stream(file);
  for (std::string line; std::getline(stream, line);) {
    if (line.find("Categories") != std::string::npos && line.find("WebBrowser") != std::string::npos) {
      return true;
    }
  }

  return false;
}

std::vector<std::string> bf::scanner::split_env(const std::string &env) {
  std::vector<std::string> result;
  std::stringstream ss(env);
  std::string item;

  while (std::getline(ss, item, ':')) {
    result.push_back(item);
  }

  return result;
}