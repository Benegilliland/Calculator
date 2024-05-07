#pragma once
#include <string>

struct constant {
  const std::string name;
  const double value;
};

bool isconstant(std::string &);
const constant *getconstant(std::string &);
