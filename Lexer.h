#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <list>

#include "Expr.h"

class Lexer {
  private:
    std::list<Token> m_tokens;
    float *m_x, *m_y, *m_t;

  public:
    Lexer(std::istream& input, float *x, float *y) :
      m_x(x), m_y(y)
    {
      scan(input);
    }

    std::list<Token>* getTokens() { return &m_tokens; }

    static void printTokens(std::list<Token>* tokens);

  private:
    void scan(std::istream& input);
    void scanString(std::istream& input);
    void scanFloat(std::istream& input);
};

#endif //LEXER_H
