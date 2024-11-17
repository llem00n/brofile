#include "brofile/browser_base.hpp"

bf::browser_base::browser_base(const std::string &executable)
    : executable(executable) {}

const std::string &bf::browser_base::get_executable() const {
  return this->executable;
}