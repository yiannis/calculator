#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "Parser.h"
#include "Parser.h"
#include "Visitor.h"

using namespace std;

typedef map<string,float>::const_iterator iter;

int main(int argc, char* argv[])
{
  int error = 0;
  map<string,float> constants;
  string source;

  if (argc == 1)
    return 1;
  if (argc >= 2)
    source = argv[1];
  if (argc >= 4)
    constants[argv[2]] = atof(argv[3]);
  if (argc >= 6)
    constants[argv[4]] = atof(argv[5]);
  if (argc > 8)
    constants[argv[6]] = atof(argv[7]);

  istringstream input(source);

  cout << source << endl;
  for (iter i=constants.begin(); i!=constants.end(); i++)
    cout << i->first << " = " << i->second << endl;

  try {
    Lexer lexer(&input);
    for (iter i=constants.begin(); i!=constants.end(); i++)
      lexer.pushConstant( i->first, &(i->second) );

    Parser parser(&lexer);

    if (parser.AST()) {
      ASTVisitorExecutor exec;
      cout << "visitor = " << parser.AST()->accept(&exec).f << endl;
    }
  } catch (ParseError e) {
    cerr << e.what() << endl;
    error = 1;
  }

  return error;
}
