#include "constant.h"
#include <cmath>

static const constant constant_list[] = {
  {"pi", std::atan(1)*4},
  {"e", std::exp(1.0)}
};

bool isconstant(std::string &token)
{
 for (const constant &cnst : constant_list) {
    if (token == cnst.name)
      return true;
  }

  return false; 
}

const constant *getconstant(std::string &token)
{
  for (const constant &cnst : constant_list) {  
    if (token == cnst.name)
      return &cnst;
  }

  return nullptr; 
}
