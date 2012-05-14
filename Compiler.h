#ifndef COMPILER_H
#define COMPILER_H

#include <iostream>
#include <cassert>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

#include "ASTBuilder.h"
#include "ASTVisitorLLVMIR.h"
#include "Parser.h"
#include "Lexer.h"

class Compiler : public ASTBuilder {
  public:
    Compiler(std::istream *input);
    ~Compiler();

    virtual void set( const std::string& name, float value );
    void compile();
    float result();
    void printIR();

  private:
    llvm::Module *m_mathModule;
    llvm::IRBuilder<> *m_mathBuilder;
    llvm::Function *m_mainFunc;
    llvm::ExecutionEngine *m_execEngine;
    ASTVisitorLLVMIR *m_llvmIR;
    std::map<std::string,int> m_constantIDs;
    std::vector<double> m_constantsVector;
    double (*MainFPtr)(double*);

    void emmitIR();
    void optimizeIR();
    void initMathFunctions();
    void initConstantIDs();
    void initConstantsVector();
};

#endif // COMPILER_H 
