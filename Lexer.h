#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <map>

#include "Expr.h"

class Lexer {
  private:
    int m_charPos;
    Token m_token;
    std::map<std::string, const float*> m_constants;
    std::istream *m_input;

  public:
    Lexer(std::istream *input) :
      m_charPos(-1), m_input(input), m_token(END, -1)
    {}

    void pushConstant( const std::string& name, const float* value )
    {
      m_constants[name] = value;
    }
    Token nextToken() { m_charPos++; m_token = scan(); return m_token; }

  private:
    Token scan();
    Token scanString();
    Token scanFloat();

    static float S_PI;
    static float S_E;
    static float S_LN2;
    static float S_LN10;
};

#endif //LEXER_H
