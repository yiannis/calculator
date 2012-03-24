#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <list>

#include "Expr.h"

enum TokenType {
  // Variables
  VARX,
  VARY,

  // Literals
  FLOAT,

  // Expressions
  EXPR,

  // Unary Operators
  NEGATIVE,

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
};

std::string tokenToString(TokenType t);

class Token {
  public:
    Token(TokenType type, ExprBase* expr = NULL) : m_type(type), m_expr(expr) {}
    ~Token() { /* Token does not own the m_expr pointer */ }

    TokenType type() const { return m_type; }
    ExprBase* expr() const { return m_expr; }
    void useAsExpr() { m_type = EXPR; }

  private:
    TokenType m_type;
    ExprBase *m_expr;
};

class Lexer {
  private:
    std::list<Token> m_tokens;
    float *m_x, *m_y;

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
