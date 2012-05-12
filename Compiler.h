#ifndef COMPILER_H
#define COMPILER_H

#include <iostream>
#include <cassert>

#include "ASTBuilder.h"
#include "ASTVisitorLLVMIR.h"
#include "Parser.h"
#include "Lexer.h"

class Compiler : public ASTBuilder {
  public:
    Compiler(std::istream *input);
    ~Compiler();

    float result();
    void emmitIR();
    void printIR();

  private:
    llvm::Module *m_mathModule;
    llvm::IRBuilder<> *m_mathBuilder;
    llvm::Function *m_mainFunc;
    ASTVisitorLLVMIR *m_llvmIR;

    void initMathFunctions();
};

#endif // COMPILER_H 
