#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "Interpreter.h"
#include "Compiler.h"

using namespace std;

typedef map<string,float>::const_iterator iter;

int main(int argc, char* argv[])
{
  int error = 0, dump = 0;
  string source;

  if (argc == 1) {
    cerr << "Usage: " << argv[0] << " [-d] <math-expression> [<var1> <value1>] [<var2> <value2>] ..." << endl;
    return 1;
  } else if (argv[1][0] == '-' && argv[1][1] == 'd') {
    dump = 1;
  }
  source = argv[1+dump];

  istringstream input(source), inputc(source);
  Interpreter engine(&input);
  Compiler llvmc(&inputc);

  int args_left = argc-2-dump;
  int pairs = args_left/2;
  int end = 2 + 2*pairs;
  for (int arg=2+dump; arg<end; arg+=2) {
    engine.set( argv[arg], atof(argv[arg+1]) );
    llvmc.set( argv[arg], atof(argv[arg+1]) );
  }
  llvmc.emmitIR();

  for (iter i=engine.getConstantsTable().begin(); i!=engine.getConstantsTable().end(); i++)
    cout << i->first << " = " << i->second << ", ";
  cout << "\n" << source << ":" << endl;

  try {
      cout << "(native)= " << engine.result() << endl;
      cout << "(llvm)= " << llvmc.result() << endl;
  } catch (exception& e) {
    cerr << e.what() << endl;
    error = 1;
  }

  if (dump)
    llvmc.printIR();

  return error;
}
