#include <iostream>
#include <cmath>

#include "Expr.h"

using namespace std;

void ExprBase::error( const std::string& message ) const
{
  throw ExprError( this, message );
}

void ExprBase::error( bool condition, const std::string& message ) const
{
  if (condition)
    throw ExprError( this, message );
}


ASTdata ExprLiteral::accept( ASTVisitor *visitor ) const
{
  return visitor->visit( this );
}

ASTdata ExprConstant::accept( ASTVisitor *visitor ) const
{
  return visitor->visit( this );
}

ASTdata ExprUnaryOp::accept( ASTVisitor *visitor ) const
{
  error( m_expr == NULL, "No argument!" );
  ASTdata data = m_expr->accept(visitor);

  return visitor->visit( this, data );
}

ASTdata ExprBinOp::accept( ASTVisitor *visitor ) const
{
  error( m_exprLeft == NULL, "No left argument!" );
  error( m_exprRight == NULL, "No right argument!" );

  ASTdata dataLeft = m_exprLeft->accept(visitor);
  ASTdata dataRight = m_exprRight->accept(visitor);

  return visitor->visit( this, dataLeft, dataRight );
}

ASTdata ExprFunction::accept( ASTVisitor *visitor ) const
{
  ASTdata data0, data1;
  switch (m_numArgs) {
    case 0:
      error( "There's no function with 0 arguments" );
      break;
    case 1:
      error( m_argv[0] == NULL, "No first argument!" );
      data0 = m_argv[0]->accept(visitor);
      return visitor->visit( this, data0 );
      break;
    case 2:
      error( m_argv[0] == NULL, "No first argument!" );
      error( m_argv[1] == NULL, "No second argument!" );
      data0 = m_argv[0]->accept(visitor);
      data1 = m_argv[1]->accept(visitor);
      return visitor->visit( this, data0, data1 );
      break;
    default:
      error( "There's no function with more than 2 arguments" );
  }
}
