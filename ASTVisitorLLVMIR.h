#ifndef AST_VISITOR_LLVMIR_H
#define AST_VISITOR_LLVMIR_H

#include <llvm/Module.h>
#include <llvm/Value.h>
#include <llvm/Support/IRBuilder.h>

#include "Visitor.h"

/// This visitor class gets an llvm Module/IRBuilder and fills the Module with llvm IR assembly from the AST.
class ASTVisitorLLVMIR: public ASTVisitor {
  public:
    virtual ASTdata visit( const ExprLiteral *literal ) const;
    virtual ASTdata visit( const ExprConstant *constant ) const;
    virtual ASTdata visit( const ExprUnaryOp *unaryOp, ASTdata data ) const;
    virtual ASTdata visit( const ExprBinOp *binaryOp, ASTdata data0, ASTdata data1 ) const;
    virtual ASTdata visit( const ExprFunction *function, ASTdata data ) const;
    virtual ASTdata visit( const ExprFunction *function, ASTdata data0, ASTdata data1 ) const;

    ASTVisitorLLVMIR( llvm::Module *mathModule, llvm::IRBuilder<> *mathBuilder ) :
      m_mathModule( mathModule ), m_mathBuilder( mathBuilder )
    {}

  private:
    llvm::Module *m_mathModule;
    llvm::IRBuilder<> *m_mathBuilder;

    llvm::Value *callFunc( const std::string& name, llvm::Value *v0 = NULL, llvm::Value *v1 = NULL ) const;
};

#endif // AST_VISITOR_LLVMIR_H
