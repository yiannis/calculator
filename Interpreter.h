#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <cassert>

#include "ASTBuilder.h"
#include "ASTVisitorExecutor.h"
#include "Parser.h"
#include "Lexer.h"

class Interpreter : public ASTBuilder {
  public:
    Interpreter(std::istream *input) :
      ASTBuilder(input)
    {}

    /// Execute the code on the AST
    float result()
    {
      return visitorResult( &m_executor ).f;
    }

  private:
    ASTVisitorExecutor m_executor;
};

#endif // INTERPRETER_H 
