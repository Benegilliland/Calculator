#include <iostream>
#include <string>
#include "parse.h"
#include "operator.h"
#include <unistd.h>
#include <iomanip>

#define DEFAULT_PRECISION 10
/*
To-do:
- add GUI
- add constants (e, pi) 
- add logarithm function
- make code safer (e.g., check stack before popping)
*/

static int precision = DEFAULT_PRECISION;
static bool printtree = false;

void
print_help(char *name)
{
  std::cout << "USAGE: " << name << "[-t -p <num> -h]\n";
  exit(EXIT_SUCCESS);
}

void
parse_opts(int argc, char *argv[])
{
  int c;

  while ((c = getopt(argc, argv, ":tp:h")) != -1) {
    switch (c) {
      case 'p':
        precision = std::stoi(optarg);
        break;
      case 't':
        printtree = true;
        break;
      case '?':
      case 'h':
      default:
        print_help(argv[0]);
    }
  }
}

int 
main(int argc, char *argv[]) 
{
  std::string infix;

  parse_opts(argc, argv);

  while (std::getline(std::cin, infix)) {
    node tree = parse_infix(infix);

    if (printtree)
      print_tree(tree, 0);

    bignum result = postfix_calculate(tree);
    std::cout << std::setprecision(precision) << result << '\n';
  }

  return 0;
}
