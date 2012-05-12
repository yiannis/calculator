#ifndef VISITOR_H
#define VISITOR_H

class ExprLiteral;
class ExprConstant;
class ExprUnaryOp;
class ExprBinOp;
class ExprFunction;

namespace llvm { class Value; }

union ASTdata {
  float f;
  llvm::Value* v;
};

class ASTVisitor {
  public:
    virtual ASTdata visit( const ExprLiteral *literal ) const = 0;
    virtual ASTdata visit( const ExprConstant *constant ) const = 0;
    virtual ASTdata visit( const ExprUnaryOp *unaryOp, ASTdata data ) const = 0;
    virtual ASTdata visit( const ExprBinOp *binaryOp, ASTdata data0, ASTdata data1 ) const = 0;
    virtual ASTdata visit( const ExprFunction *function, ASTdata data ) const = 0;
    virtual ASTdata visit( const ExprFunction *function, ASTdata data0, ASTdata data1 ) const = 0;
};

#endif // VISITOR_H
