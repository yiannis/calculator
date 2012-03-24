#include <string>

#include "Token.h"

using namespace std;

string tokenToString(TokenType t)
{
  switch (t) {
    case VARX:
      return "x";
    case VARY:
      return "y";
    case FLOAT:
      return "float";
    case EXPR:
      return "<expr>";
    case PLUS:
      return "+";
    case MINUS:
    case NEGATIVE:
      return "-";
    case MULT:
      return "*";
    case DIV:
      return "/";
    case POWER:
      return "^";
    case FUNCTION:
      return "<func>";
    case LPAREN:
      return "(";
    case RPAREN:
      return ")";
    case COMMA:
      return ",";
    default:
      return "unknown";
  }
}
