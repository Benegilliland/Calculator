#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cmath>
#include <gmpxx.h>

/*
To-do:
- add scale variable
- add GUI
- add constants (e, pi) 
- add logarithm function
- make code safer (e.g., check stack before popping)
*/

typedef mpf_class number;
typedef number (*unary_operation)(number);
typedef number (*binary_operation)(number, number);

struct Operator 
{
  char c;
  unsigned int precedence;
  unary_operation un_op = nullptr;
  binary_operation bin_op = nullptr;
};

// Function prototypes
std::string infix_to_postfix(std::string);
number postfix_calculate(std::string);
const Operator *isoperator(char);

number add(number, number);
number subtract(number, number);
number multiply(number, number);
number divide(number, number);
number fact(number);

const Operator operator_list[] = {
  {'+', 1, nullptr, add },
  {'-', 1, nullptr, subtract },
  {'*', 2, nullptr, multiply },
  {'/', 2, nullptr, divide },
  //{'^', 3, nullptr, [](number left, number right) {return (number)pow(left, right);} },
  //{'l', 4, nullptr, [](number left, number right) {return (number)(log(left) / log(left));} },
  {'!', 4, fact, nullptr},
  {'(', 0},
  {')', 1}
};

int 
main() 
{
  std::string infix;

  while (std::getline(std::cin, infix)) {
    std::string postfix;
    number result;
    
    postfix = infix_to_postfix(infix);
    std::cout << infix_to_postfix(infix) << '\n';

    result = postfix_calculate(postfix);
    std::cout << result << '\n';
  }

  return 0;
}

number add(number left, number right)
{
  return left + right;
}

number multiply(number left, number right)
{
  return left * right;
}

number subtract(number left, number right)
{
  return left - right;
}

number divide(number left, number right)
{
  return left / right;
}

number fact(number n)
{
  return (number)factorial((mpz_class)n);
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

std::string
infix_to_postfix(std::string infix)
{
  std::string postfix = "";
  std::stack<const Operator *> operators;

  for (char c : infix) {
    const Operator *op;

    if (std::isdigit(c) || std::isspace(c) || c == '.') {
      postfix += c;
    }
    else if ((op = isoperator(c))) {
      if (op->c == '(') {
        operators.push(op);
        continue;
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
      
  }
  
  while (!operators.empty()) {
    postfix += operators.top()->c;
    operators.pop();
  }

  return postfix;
}

number
postfix_calculate(std::string postfix)
{
  number num = 0;
  bool digit = false;
  static std::stack<number> nums;
  size_t startPos, len;

  for (size_t i = 0; i < postfix.length(); i++) {
    char c = postfix[i];

    if (std::isdigit(c) or c == '.') {
      if (!digit) {
        digit = true;
        startPos = i;
        len = 0;
      }

      len++;
    }
    else {
      if (digit) {
        num = number(postfix.substr(startPos, len));
        nums.push(num);
        digit = false;
      }

      const Operator *op;
      if ((op = isoperator(c))) {
        number result = 0;
        number left = nums.top();
        nums.pop();

        if (op->un_op) {
          result = op->un_op(left);
        }
        else {
          number right = nums.top();
          nums.pop();
          result = op->bin_op(right, left);
        }

        nums.push(result);
      }
    }

  }
  
  if (digit) {
    num = number(postfix.substr(startPos, len));
    nums.push(num);
  }

  if (!nums.empty())
    return nums.top();
  else
    return 0;
}
