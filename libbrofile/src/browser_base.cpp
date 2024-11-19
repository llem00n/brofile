#include "brofile/browser_base.hpp"

bf::browser_base::browser_base(const std::string &executable, const browser_type &type)
    : executable(executable), type(type) {}

const std::string &bf::browser_base::get_executable() const { return this->executable; }

const bf::browser_type &bf::browser_base::get_type() const { return this->type; }
