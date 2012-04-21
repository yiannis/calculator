#include <string>

#include "Token.h"

using namespace std;


const std::string Token::tokenToString(TokenType t)
{
  switch (t) {
    case FLOAT:
      return "<float>";
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
