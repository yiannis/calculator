#ifndef TOKEN_H
#define TOKEN_H

#include <sstream>

#include "Function.h"

enum TokenType {
  // User defined or builtin constants
  CONSTANT,

  // Literals
  FLOAT,

  // Expressions
  EXPR,

  // Unary Operators
  NEGATIVE,
  POSITIVE,
  FACTORIAL,

  // Binary Operators
  PLUS,
  MINUS,
  MULT,
  DIV,
  POWER,

  // Function with 1 or 2 arguments
  FUNCTION,

  // Separators
  LPAREN,
  RPAREN,
  COMMA,

  // EOF
  END,
};


struct Token {
  Token(TokenType type, int pos) :
    m_type(type), m_pos(pos)
  {}
  Token( Function::ID id, int pos ) :
    m_type(FUNCTION), m_pos(pos)
  {
    m_data.id = id;
  }
  Token( float value, int pos ) :
    m_type(FLOAT), m_pos(pos)
  {
    m_data.value = value;
  }
  Token( float *pvalue, int pos ) :
    m_type(CONSTANT), m_pos(pos)
  {
    m_data.pValue = pvalue;
  }

  std::string toString() const
  {
    std::string str;
    std::ostringstream out;

    switch (m_type) {
      case FUNCTION:
        str = Function::IDtoName( m_data.id );
        break;
      case FLOAT:
        out << "(" << m_data.value << ")";
        str = out.str();
        break;
      case CONSTANT:
        out << "(" << *(m_data.pValue) << ")";
        str = out.str();
        break;
      default:
        str = tokenToString(m_type);
    }

    return str;
  }

  static const std::string tokenToString(TokenType t);

  TokenType m_type;
  int m_pos;
  union {
    Function::ID id;
    float value;
    float* pValue;
  } m_data;
};

#endif //TOKEN_H
