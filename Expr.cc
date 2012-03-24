#include <iostream>
#include <cmath>

#include "Expr.h"

using namespace std;

float ExprUnaryOp::result()
{
  float expr = m_expr->result();

  switch (m_op) {
    case '-':
      return -expr;
  }
}

float ExprBinOp::result()
{
  float l = m_expr_left->result();
  float r = m_expr_right->result();

  switch (m_op) {
    case '+':
      return l+r;
    case '-':
      return l-r;
    case '*':
      return l*r;
    case '/':
      return l/r;
    case '^':
      return pow(l, r);
  }
}

float ExprFunction::result()
{
  float result = 0.0F;
  if (m_arg1) {
    if (m_arg2) {
      result = Function::call(m_id, m_arg1->result(), m_arg2->result());
    } else {
      result = Function::call(m_id, m_arg1->result());
    }
  } else {
    cerr << __func__ << "(): " << __LINE__ << ": Error: Calling a function without arguments" << endl;
  }

  return result;
}