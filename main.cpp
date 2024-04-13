#include <iostream>
#include <brofile/scanner.hpp>
#include "brofile/browser_factory.hpp"

int main() {
  auto files = bf::scanner::scan_default();
  for (const auto& file : files) {
    std::cout << file << std::endl;
  }

  auto chrome = bf::browser_factory::create(files[1]);
  std::cout << chrome->get_browser_name() << std::endl;
  chrome->set_url("https://www.google.com");
  chrome->set_profile(chrome->get_profiles()[1]);
  chrome->set_new_window(true);

  chrome->open();

  auto profiles = chrome->get_profiles();
  for (const auto& profile : profiles) {
    std::cout << profile.name << ": " << profile.path << std::endl;
  }

  return 0;
}
