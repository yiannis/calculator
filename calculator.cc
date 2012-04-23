#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "Interpreter.h"

using namespace std;

typedef map<string,float>::const_iterator iter;

int main(int argc, char* argv[])
{
  int error = 0;
  string source;

  if (argc == 1) {
    cerr << "Usage: " << argv[0] << "<math-expression> [<var1> <value1>] [<var2> <value2>] ..." << endl;
    return 1;
  } else {
    source = argv[1];
  }

  istringstream input(source);
  Interpreter engine(&input);

  int args_left = argc-2;
  int pairs = args_left/2;
  int end = 2 + 2*pairs;
  for (int arg=2; arg<end; arg+=2)
    engine.set( argv[arg], atof(argv[arg+1]) );

  for (iter i=engine.getConstantsTable().begin(); i!=engine.getConstantsTable().end(); i++)
    cout << i->first << " = " << i->second << ", ";
  cout << "\n" << source;

  try {
    if (engine.parse()) {
      cout << " = " << engine.result() << endl;
    }
  } catch (exception& e) {
    cerr << e.what() << endl;
    error = 1;
  }

  return error;
}
