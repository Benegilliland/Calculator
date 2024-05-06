#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cmath>

/*
To-do:
- arbitrary precision
- add support for decimal numbers
- add scale variable
- add GUI
- add constants (e, pi) 
- add logarithm function
*/

typedef double number;
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

number factorial(number);

const Operator operator_list[] = {
  {'+', 1, nullptr, [](number left, number right) {return left + right;} },
  {'-', 1, nullptr, [](number left, number right) {return left - right;} },
  {'*', 2, nullptr, [](number left, number right) {return left * right;} },
  {'/', 2, nullptr, [](number left, number right) {return left / right;} },
  {'^', 3, nullptr, [](number left, number right) {return (number)pow(left, right);} },
  {'l', 4, nullptr, [](number left, number right) {return (number)(log(left) / log(left));} },
  {'!', 4, factorial, nullptr},
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
    result = postfix_calculate(postfix);

    std::cout << infix_to_postfix(infix) << '\n';
    std::cout << result << '\n';
  }

  return 0;
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

number factorial(number n)
{
  return (n <= 1 ? 1 : n * factorial(n - 1));
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
    else if (op = isoperator(c)) {
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
  long long num = 0;
  bool digit = false;
  static std::stack<number> nums;

  for (char c : postfix) {

    if (std::isdigit(c)) {
      num = 10 * num + (c - '0');
      digit = true;
    }
    else {
      if (digit) {
        nums.push(num);
        num = 0;
        digit = false;
      }

      const Operator *op;
      if (op = isoperator(c)) {
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
  
  if (num != 0)
    nums.push(num);

  if (!nums.empty())
    return nums.top();
  else
    return 0;
}
