#pragma once
#include <string>
#include <stack>
#include <cctype>
#include "operator.h"

typedef struct s_token {
  const Operator *op;
  bignum *num;

  s_token *left = nullptr;
  s_token *right = nullptr;
} *node;

node parse_infix(const std::string &infix);
bignum postfix_calculate(node tree);
void print_tree(node tree, int h);
