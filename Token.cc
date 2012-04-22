#include <string>

#include "Token.h"

using namespace std;


const std::string Token::tokenToString(TokenType t)
{
  switch (t) {
    case CONSTANT:
      return "<const>";
    case FLOAT:
      return "<float>";
    case EXPR:
      return "<expr>";
    case FACTORIAL:
      return "!";
    case MINUS:
    case NEGATIVE:
      return "-";
    case PLUS:
    case POSITIVE:
      return "+";
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
    case END:
      return "<eof>";
    default:
      return "unknown";
  }
}
