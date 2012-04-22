#ifndef VISITOR_H
#define VISITOR_H

class ExprLiteral;
class ExprConstant;
class ExprUnaryOp;
class ExprBinOp;
class ExprFunction;

union ASTdata {
  float f;
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

class ASTVisitorExecutor : public ASTVisitor {
  public:
    virtual ASTdata visit( const ExprLiteral *literal ) const;
    virtual ASTdata visit( const ExprConstant *constant ) const;
    virtual ASTdata visit( const ExprUnaryOp *unaryOp, ASTdata data ) const;
    virtual ASTdata visit( const ExprBinOp *binaryOp, ASTdata data0, ASTdata data1 ) const;
    virtual ASTdata visit( const ExprFunction *function, ASTdata data ) const;
    virtual ASTdata visit( const ExprFunction *function, ASTdata data0, ASTdata data1 ) const;
};

#endif //VISITOR_H
