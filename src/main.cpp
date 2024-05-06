#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <cctype>
#include <cmath>
#include <gmpxx.h>
#include <iomanip>

/*
To-do:
- add scale variable
- add GUI
- add constants (e, pi) 
- add logarithm function
- make code safer (e.g., check stack before popping)
*/

// bigint for arbitrary size integers, bignum for arbitrary size floats
typedef mpz_class bigint;
typedef mpf_class bignum;
typedef bignum (*unary_operation)(bignum);
typedef bignum (*binary_operation)(bignum, bignum);

struct Operator 
{
  char c;
  unsigned int precedence;
  unary_operation un_op = nullptr;
  binary_operation bin_op = nullptr;
};

// Function prototypes
std::string infix_to_postfix(std::string, std::queue<bignum>&);
bignum postfix_calculate(std::string, std::queue<bignum>&);
const Operator *isoperator(char);

bignum add(bignum, bignum);
bignum subtract(bignum, bignum);
bignum multiply(bignum, bignum);
bignum divide(bignum, bignum);
bignum fact(bignum);

const Operator operator_list[] = {
  {'+', 1, nullptr, add },
  {'-', 1, nullptr, subtract },
  {'*', 2, nullptr, multiply },
  {'/', 2, nullptr, divide },
  //{'^', 3, nullptr, [](bignum left, bignum right) {return (bignum)pow(left, right);} },
  //{'l', 4, nullptr, [](bignum left, bignum right) {return (bignum)(log(left) / log(left));} },
  {'!', 4, fact, nullptr},
  {'(', 0},
  {')', 1}
};

int 
main() 
{
  std::string infix;
  std::queue<bignum> nums;

  while (std::getline(std::cin, infix)) {
    std::string postfix;
    bignum result;
    
    postfix = infix_to_postfix(infix, nums);
    std::cout << postfix << '\n';

    result = postfix_calculate(postfix, nums);
    std::cout << std::setprecision(1000) << result << '\n';
  }

  return 0;
}

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

bignum fact(bignum n)
{
  bigint i = (bigint)n;
  return (bignum)factorial(i);
}

const Operator * 
isoperator(char c)
{
  for (const Operator &op : operator_list) {
    if (op.c == c)
      return &op;
  }

  return nullptr;
}

void getnum(std::string &str, std::queue<bignum> &nums, size_t startPos, size_t len)
{
  bignum num = bignum(str.substr(startPos, len));
  nums.push(num);
  std::cout << "num: " << num << '\n';
}

void parse_operator(std::string &postfix, const Operator *op, std::stack<const Operator *> &operators)
{
  if (op->c == '(') {
    operators.push(op);
    return;
  }
        
  while (!operators.empty() && operators.top()->precedence >= op->precedence) {  
    postfix += operators.top()->c;
    operators.pop();
  }

  if (op->c == ')')
    operators.pop();
  else
    operators.push(op);
}

std::string
infix_to_postfix(std::string infix, std::queue<bignum>& nums)
{
  std::string postfix = "";
  std::stack<const Operator *> operators;
  bool digit = false;
  size_t startPos, len;
  
  for (size_t i = 0; i < infix.length(); i++) {
    const Operator *op;
    char c = infix[i];

    if (std::isdigit(c) || c == '.') {
      if (!digit) {
        digit = true;
        startPos = i;
        len = 0;
        postfix += 'n';
      }

      len++;
    }
    else {
      if (digit) {
        digit = false;
        getnum(infix, nums, startPos, len);
      }

      if ((op = isoperator(c)))
        parse_operator(postfix, op, operators);
    }
  }

  if (digit)
    getnum(infix, nums, startPos, len);

  while (!operators.empty()) {
    postfix += operators.top()->c;
    operators.pop();
  }

  return postfix;
}

void do_operation(std::stack<bignum> &stack, const Operator *op)
{
  bignum result;

  if (op->un_op) {
    bignum left = stack.top();
    stack.pop();

    result = op->un_op(left);
  }
        
  else if (op->bin_op) {
    bignum left = stack.top();
    stack.pop();
    bignum right = stack.top();
    stack.pop();

    result = op->bin_op(right, left);
  }

  stack.push(result);
}

bignum
postfix_calculate(std::string postfix, std::queue<bignum>& nums)
{
  static std::stack<bignum> num_stack;

  for (char c : postfix) {

    if (c == 'n') {
      num_stack.push(nums.front());
      nums.pop();
    }
    else {
      const Operator *op;

      if ((op = isoperator(c)))
        do_operation(num_stack, op);
    }

  } 

  if (!num_stack.empty())
    return num_stack.top();
  else
    return 0;
}
