#include <iostream>
#include "parse.h"
#include "operator.h"

/*
To-do:
- add scale variable
- add GUI
- add constants (e, pi) 
- add logarithm function
- make code safer (e.g., check stack before popping)
*/

int 
main() 
{
  std::string infix;

  while (std::getline(std::cin, infix)) {
    node tree = parse_infix(infix);
    print_tree(tree, 0);
    bignum result = postfix_calculate(tree);
    std::cout << result << '\n';
  }

  return 0;
}
