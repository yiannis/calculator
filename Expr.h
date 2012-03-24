#ifndef EXPR_H
#define EXPR_H

#include <string>

#include "Function.h"
#include "Token.h"

class ExprBase {
  public:
    virtual float result() = 0;
};

class ExprLiteral : public ExprBase {
  private:
    float m_number;

  public:
    ExprLiteral(float number) : m_number(number) {}

    virtual float result() { return m_number; }
};

class ExprVariable : public ExprBase {
  private:
    float *m_number;

  public:
    ExprVariable(float *number) : m_number(number) {}

    virtual float result() { return *m_number; }
};

class ExprUnaryOp : public ExprBase {
  private:
    TokenType m_op;
    ExprBase *m_expr;

  public:
    ExprUnaryOp(TokenType op, ExprBase* expr = NULL)
      : m_op(op), m_expr(expr)
    {}

    void setArgs(ExprBase* expr) { m_expr = expr; }

    virtual float result();
};

class ExprBinOp : public ExprBase {
  private:
    TokenType m_op;
    ExprBase *m_expr_left, *m_expr_right;

  public:
    ExprBinOp(TokenType op, ExprBase* left = NULL, ExprBase* right = NULL)
      : m_op(op), m_expr_left(left), m_expr_right(right)
    {}

    void setArgs(ExprBase* left, ExprBase* right)
    {
      m_expr_left = left;
      m_expr_right = right;
    }

    virtual float result();
};

class ExprFunction : public ExprBase {
  private:
    Function::ID m_id;
    ExprBase *m_arg1, *m_arg2;

  public:
    ExprFunction(std::string name, ExprBase* arg1 = NULL, ExprBase* arg2 = NULL) :
      m_arg1(arg1), m_arg2(arg2)
    {
      m_id = Function::nameToID(name);
    }

    void setArgs(ExprBase* arg1, ExprBase* arg2 = NULL)
    {
      m_arg1 = arg1;
      m_arg2 = arg2;
    }

    std::string name() { return Function::IDtoName(m_id); }

    virtual float result();
};

#endif //EXPR_H
