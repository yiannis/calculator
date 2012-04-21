#include <iostream>
#include <cmath>

#include "Expr.h"

using namespace std;

void ExprBase::error( const std::string& message ) const
{
  throw ExprError( this, message );
}

float ExprUnaryOp::result() const
{
  float expr = m_expr->result();

  switch (type()) {
    case NEGATIVE:
      return -expr;
    default:
      error( "No such unary operator" );
  }
}

float ExprBinOp::result() const
{
  float l = m_exprLeft->result();
  float r = m_exprRight->result();

  switch (type()) {
    case PLUS:
      return l+r;
    case MINUS:
      return l-r;
    case MULT:
      return l*r;
    case DIV:
      return l/r;
    case POWER:
      return pow(l, r);
    default:
      error( "No such binary operator" );
  }
}

float ExprFunction::result() const
{
  float result = 0.0F;
  switch (m_numArgs) {
    case 0:
      error( "There's no function with 0 arguments" );
      break;
    case 1:
      result = Function::call(m_token.m_data.id, m_argv[0]->result());
      break;
    case 2:
      result = Function::call(m_token.m_data.id, m_argv[0]->result(), m_argv[1]->result());
      break;
    default:
      error( "There's no function with more than 2 arguments" );
  }

  return result;
}
