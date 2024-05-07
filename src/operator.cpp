#include "operator.h"
#include <cmath>

static const Operator operator_list[] = {
  {'+', 1, add, Operator::binary, Operator::left },
  {'-', 1, subtract, Operator::binary, Operator::left },
  {'*', 2, multiply, Operator::binary, Operator::left },
  {'/', 2, divide, Operator::binary, Operator::left },
  {'^', 3, power, Operator::binary, Operator::left },
  //{'l', 4, nullptr, [](bignum left, bignum right) {return (bignum)(log(left) / log(left));} },
  {'!', 4, fact, Operator::unary, Operator::left},
  {'(', 0, nullptr, Operator::binary, Operator::left},
  {')', 1, nullptr, Operator::binary, Operator::left}
};

bignum add(bignum left, bignum right)
{
  return left + right;
}

bignum multiply(bignum left, bignum right)
{
  return left * right;
}

bignum subtract(bignum left, bignum right)
{
  return left - right;
}

bignum divide(bignum left, bignum right)
{
  return left / right;
}

bignum fact(bignum left, bignum right)
{
  bigint i = (bigint)left;
  return (bignum)factorial(i);
}

bignum power(bignum left, bignum right)
{
  double d_left = left.get_d();
  double d_right = right.get_d();
  return (bignum)pow(d_left, d_right);
}

bool
isoperator(char c)
{
  for (const Operator &op : operator_list) {
    if (op.c == c)
      return true;
  }

  return false;
}

const Operator *
getoperator(std::string &token)
{
  char c = token[0];

  for (const Operator &op : operator_list) {
    if (op.c == c)
      return &op;
  }

  return nullptr;
}
