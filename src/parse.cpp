#include "parse.h"
#include <stack>
#include <iostream>

static std::stack<const Operator *> ops;
static std::stack<node> nodes;

static char charType(char c)
{
  if (std::isdigit(c) || c == '.')
    return 'n';
  else if (std::isalpha(c))
    return 'a';
  else if (std::isspace(c))
    return ' ';
  else if (isoperator(c))
    return 'o';
  else
    return 'e';
}

void parse_number(std::string &token)
{
  bignum *num = new bignum(token);
  node newNode = new s_token{nullptr, num, nullptr, nullptr};
  nodes.push(newNode);
}

void output_operator(const Operator *op)
{
  node left = nullptr, right = nullptr;

  if (op->type == Operator::unary) {
    left = nodes.top();
    nodes.pop();
  }
  else if (op->type == Operator::binary) {
    right = nodes.top();
    nodes.pop();
    left = nodes.top();
    nodes.pop();
  }

  node newNode = new s_token{op, nullptr, left, right};
  nodes.push(newNode);
}

void parse_operator(std::string &token)
{
  const Operator *op = getoperator(token);

  if (op->c == '(') {
    ops.push(op);
    return;
  }

  while (!ops.empty() && ops.top()->precedence >= op->precedence) {
    const Operator *next = ops.top();
    ops.pop();
    output_operator(next);
  }

  if (op->c == ')')
    ops.pop();
  else
    ops.push(op);
}

void empty_operator_stack()
{
  while (!ops.empty()) {
    const Operator *next = ops.top();
    ops.pop();
    output_operator(next);
  }
}

void parse_token(std::string &token, char type)
{
  switch (type) {
    case 'n':
      parse_number(token);
      break;
    case 'a':
      //parse_alpha();
      break;
    case 'o':
      parse_operator(token);
      break;
    case 'e':
      //parse_error();
      break;
  }
}

node parse_infix(const std::string &infix)
{
  char prevType = charType(infix[0]);
  size_t startPos = 0, len = 0;
  std::string token;

  for (size_t i = 0; i < infix.length(); i++) {
    char curType = charType(infix[i]);
    
    if (curType != prevType) {
      token = infix.substr(startPos, len);
      parse_token(token, prevType);
      startPos = i;
      len = 0;
      prevType = curType;
    }
    
    len++;
  }

  token = infix.substr(startPos, len);
  parse_token(token, prevType);
  empty_operator_stack();
  return nodes.top(); 
}

void printNode(node n, int h)
{
  std::cout << std::string(2 * h, ' ');
  if (n->op)
    std::cout << n->op->c;
  else if (n->num)
    std::cout << *n->num;
  std::cout << "\n";
}

void print_tree(node tree, int h)
{
  if (tree == nullptr) return;
  printNode(tree, h);
  print_tree(tree->left, h + 1);
  print_tree(tree->right, h + 1);
}

bignum postfix_calculate(node n)
{
  if (n == nullptr)
      return 0;
  else if (n->num) 
    return *n->num;
  else if (n->op) 
    return n->op->op(postfix_calculate(n->left), postfix_calculate(n->right));
}
