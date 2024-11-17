#include <brofile_private/tools/ini_tools.hpp>

std::map<std::string, std::map<std::string, std::string>>
bf::tools::parse_simple_ini(std::ifstream &file) {
  std::map<std::string, std::map<std::string, std::string>> result;
  std::string line;
  std::string current_section;

  while (std::getline(file, line)) {
    line.erase(0, line.find_first_not_of(" \t"));
    line.erase(line.find_last_not_of(" \t") + 1);
    if (line.empty() || line[0] == ';' || line[0] == '#') {
      continue;
    }

    if (line[0] == '[' && line.back() == ']') {
      current_section = line.substr(1, line.size() - 2);
      result[current_section] = std::map<std::string, std::string>();
    } else if (!current_section.empty() &&
               line.find('=') != std::string::npos) {
      size_t delimiter_pos = line.find('=');
      std::string key = line.substr(0, delimiter_pos);
      std::string value = line.substr(delimiter_pos + 1);

      key.erase(0, key.find_first_not_of(" \t"));
      key.erase(key.find_last_not_of(" \t") + 1);
      value.erase(0, value.find_first_not_of(" \t"));
      value.erase(value.find_last_not_of(" \t") + 1);

      result[current_section][key] = value;
    }
  }

  return result;
}

std::map<std::string, std::map<std::string, std::string>>
bf::tools::parse_simple_ini(std::ifstream &&file) {
  return parse_simple_ini(file);
}