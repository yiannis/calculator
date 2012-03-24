#include <iostream>
#include <sstream>
#include <cstdlib>

#include "Parser.h"
#include "Lexer.h"

using namespace std;

int main(int argc, char* argv[])
{
  float x = 0, y = 0;

  if (argc == 1)
    return 1;
  if (argc > 2)
    x = atof(argv[2]);
  if (argc > 3)
    y = atof(argv[3]);

  cout << endl;
  string function3 = argv[1];
  istringstream input(function3);
  Lexer lexer(input, &x, &y);
  Parser parser(lexer.getTokens());
  if (parser.OK())
    cout << function3 << " (x="<<x<<",y="<<y<<") = " << parser.getAST()->result() << endl;

  return 0;
}
