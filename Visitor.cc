#include <cmath>

#include "Visitor.h"
#include "Expr.h"

#ifdef DEBUG
# include <cstdio>
# define DBG fprintf( stderr, "%s: got %f\n", expr->toString().c_str(), data.f );
#else
# define DBG
#endif

ASTdata ASTVisitorExecutor::visit( const ExprLiteral *expr ) const
{
  ASTdata data;
  data.f = expr->value();
  return data;
}


ASTdata ASTVisitorExecutor::visit( const ExprConstant *expr ) const
{
  ASTdata data;
  data.f = expr->value();
  return data;
}

ASTdata ASTVisitorExecutor::visit( const ExprUnaryOp *expr, ASTdata data0 ) const
{
  ASTdata data;
  switch (expr->type()) {
    case NEGATIVE:
      data.f = -data0.f;
      break;
    default:
      expr->error( "No such unary operator" );
  }

  return data;
}

ASTdata ASTVisitorExecutor::visit( const ExprBinOp *expr, ASTdata data0, ASTdata data1 ) const
{
  ASTdata data;
  switch (expr->type()) {
    case PLUS:
      data.f = data0.f + data1.f;
      break;
    case MINUS:
      data.f = data0.f - data1.f;
      break;
    case MULT:
      data.f = data0.f * data1.f;
      break;
    case DIV:
      data.f = data0.f / data1.f;
      break;
    case POWER:
      data.f = pow( data0.f, data1.f );
      break;
    default:
      expr->error( "No such binary operator" );
  }

  return data;
}

ASTdata ASTVisitorExecutor::visit( const ExprFunction *expr, ASTdata data0 ) const
{
  ASTdata data;
  data.f = Function::call(expr->id(), data0.f);
  return data;
}

ASTdata ASTVisitorExecutor::visit( const ExprFunction *expr, ASTdata data0, ASTdata data1 ) const
{
  ASTdata data;
  data.f = Function::call(expr->id(), data0.f, data1.f);
  return data;
}

#undef DBG
