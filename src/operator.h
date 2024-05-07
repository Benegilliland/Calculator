#pragma once
#include <cmath>
#include <gmpxx.h>
#include <string>

// bigint for arbitrary size integers, bignum for arbitrary size floats
typedef mpz_class bigint;
typedef mpf_class bignum;
typedef bignum (*binary_operation)(bignum, bignum);

struct Operator 
{
  char c;
  unsigned int precedence;
  binary_operation op;
  
  enum {unary, binary} type;
  enum {left, right} associativity;
};

bignum add(bignum, bignum);
bignum subtract(bignum, bignum);
bignum multiply(bignum, bignum);
bignum divide(bignum, bignum);
bignum power(bignum, bignum);
bignum fact(bignum, bignum);

bool isoperator(char);
const Operator *getoperator(std::string &token);
bignum run_operation(const Operator *op, bignum left, bignum right);
