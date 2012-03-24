#include <iostream>
#include <cmath>

#include "Function.h"

using namespace std;

Function::ID Function::nameToID(string name)
{
  if (name == "sin")
    return SIN;
  else if (name == "cos")
    return COS;
  else if (name == "tan")
    return TAN;
  else if (name == "acos")
    return ACOS;
  else if (name == "asin")
    return ASIN;
  else if (name == "atan")
    return ATAN;
  else if (name == "exp")
    return EXP;
  else if (name == "log")
    return LOG;
  else if (name == "log10")
    return LOG10;
  else if (name == "sqrt")
    return SQRT;
  else if (name == "pow")
    return POW;
  else if (name == "atan2")
    return ATAN2;
  else
    return UNKNOWN;
}

string Function::IDtoName(Function::ID id)
{
  switch (id) {
    case SIN:
      return "sin";
    case COS:
      return "cos";
    case TAN:
      return "tan";
    case ACOS:
      return "acos";
    case ASIN:
      return "asin";
    case ATAN:
      return "atan";
    case EXP:
      return "exp";
    case LOG:
      return "log";
    case LOG10:
      return "log10";
    case SQRT:
      return "sqrt";
    case POW:
      return "pow";
    case ATAN2:
      return "atan2";
    default:
      return "unknown";
  }
}

float Function::call(Function::ID id, float arg)
{
  switch (id) {
    case SIN:
      return sin(arg);
    case COS:
      return cos(arg);
    case TAN:
      return tan(arg);
    case ACOS:
      return acos(arg);
    case ASIN:
      return asin(arg);
    case ATAN:
      return atan(arg);
    case EXP:
      return exp(arg);
    case LOG:
      return log(arg);
    case LOG10:
      return log10(arg);
    case SQRT:
      return sqrt(arg);
    default:
      cerr << __func__ << "(): " << __LINE__ << " :Unknown id '" << id << "'" << endl;
      return 0.0F;
  }
}

float Function::call(Function::ID id, float arg1, float arg2)
{
  switch (id) {
    case POW:
      return pow(arg1, arg2);
    case ATAN2:
      return atan2(arg1, arg2);
    default:
      cerr << __func__ << "(): " << __LINE__ << " :Unknown id '" << id << "'" << endl;
      return 0.0F;
  }
}
