#ifndef TOKEN_H
#define TOKEN_H

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

class ExprBase;

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

#endif //TOKEN_H
