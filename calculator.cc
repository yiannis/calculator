#include <iostream>
#include <sstream>
#include <cstdlib>

#include "Parser.h"
#include "Lexer.h"
#include "Executor.h"

using namespace std;

int main(int argc, char* argv[])
{
  Executor code;

  if (argc == 1)
    return 1;
  if (argc > 2)
    code.x() = atof(argv[2]);
  if (argc > 3)
    code.y() = atof(argv[3]);

  cout << endl;
  string function3 = argv[1];
  istringstream input(function3);
  Lexer lexer(input, code.x_p(), code.y_p());
  Parser parser(lexer.getTokens());
  if (parser.OK())
    cout << function3 << " (x="<<code.x()<<",y="<<code.y()<<") = " << code.setAST( parser.getAST() ).run() << endl;

  return 0;
}
