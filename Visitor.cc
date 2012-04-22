#include <cmath>

#include "Visitor.h"
#include "Expr.h"


ASTdata ASTVisitorExecutor::visit( const ExprLiteral *literal ) const
{
  ASTdata data;
  data.f = literal->value();
  return data;
}


ASTdata ASTVisitorExecutor::visit( const ExprConstant *constant ) const
{
  ASTdata data;
  data.f = constant->value();
  return data;
}

ASTdata ASTVisitorExecutor::visit( const ExprUnaryOp *unaryOp, ASTdata data0 ) const
{
  ASTdata data;
  switch (unaryOp->type()) {
    case NEGATIVE:
      data.f = -data0.f;
      break;
    default:
      unaryOp->error( "No such unary operator" );
  }

  return data;
}

ASTdata ASTVisitorExecutor::visit( const ExprBinOp *binaryOp, ASTdata data0, ASTdata data1 ) const
{
  ASTdata data;
  switch (binaryOp->type()) {
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
      binaryOp->error( "No such binary operator" );
  }

  return data;
}

ASTdata ASTVisitorExecutor::visit( const ExprFunction *function, ASTdata data0 ) const
{
  ASTdata data;
  data.f = Function::call(function->id(), data0.f);
  return data;
}

ASTdata ASTVisitorExecutor::visit( const ExprFunction *function, ASTdata data0, ASTdata data1 ) const
{
  ASTdata data;
  data.f = Function::call(function->id(), data0.f, data1.f);
  return data;
}
