#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "Expr.h"

/// An Abstact Syntax Tree wrapper
class Executor {
  public:
    Executor(): m_tree(NULL), m_x(0.0F), m_y(0.0F), m_t(0.0F) {}

    //TODO Error checking

    Executor& setAST(ExprBase* ast) { m_tree = ast; return *this; }

    float& x() { return m_x; }
    float& y() { return m_y; }
    float& t() { return m_t; }

    float* x_p() { return &m_x; }
    float* y_p() { return &m_y; }
    float* t_p() { return &m_t; }

    /// Execute the code on the AST
    //float run() { return m_tree->result(); }

  private:
    ExprBase* m_tree;
    float m_x, m_y, m_t;
};

#endif // EXECUTOR_H
