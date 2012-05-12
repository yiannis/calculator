#ifndef VISITOR_EXECUTOR_H
#define VISITOR_EXECUTOR_H

#include "Visitor.h"

class ASTVisitorExecutor : public ASTVisitor {
  public:
    virtual ASTdata visit( const ExprLiteral *literal ) const;
    virtual ASTdata visit( const ExprConstant *constant ) const;
    virtual ASTdata visit( const ExprUnaryOp *unaryOp, ASTdata data ) const;
    virtual ASTdata visit( const ExprBinOp *binaryOp, ASTdata data0, ASTdata data1 ) const;
    virtual ASTdata visit( const ExprFunction *function, ASTdata data ) const;
    virtual ASTdata visit( const ExprFunction *function, ASTdata data0, ASTdata data1 ) const;
};

#endif // VISITOR_EXECUTOR_H
