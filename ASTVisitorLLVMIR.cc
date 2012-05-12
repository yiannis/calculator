#include <llvm/DerivedTypes.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>

#include <cstdio>
#include <string>
#include <vector>

#include "ASTVisitorLLVMIR.h"
#include "Expr.h"

#ifdef DEBUG
# include <cstdio>
# define DBG fprintf( stderr, "%s: got %f\n", expr->toString().c_str(), data.f );
#else
# define DBG
#endif

using namespace std;
using llvm::ConstantFP;
using llvm::getGlobalContext;
using llvm::APFloat;
using llvm::Value;

/// ASTVisitorLLVMIR ///
ASTdata ASTVisitorLLVMIR::visit( const ExprLiteral *expr ) const
{
  ASTdata data;
  data.v = ConstantFP::get(getGlobalContext(), APFloat(static_cast<double>(expr->value())));
  return data;
}

ASTdata ASTVisitorLLVMIR::visit( const ExprConstant *expr ) const
{
  ASTdata data;
  data.v = ConstantFP::get(getGlobalContext(), APFloat(static_cast<double>(expr->value())));
  return data;
}

ASTdata ASTVisitorLLVMIR::visit( const ExprUnaryOp *expr, ASTdata data0 ) const
{
  ASTdata data;
  switch (expr->type()) {
    case NEGATIVE:
      data.v = m_mathBuilder->CreateFNeg(data0.v, "negative");
      break;
    default:
      expr->error( "No such unary operator" );
  }

  return data;
}

ASTdata ASTVisitorLLVMIR::visit( const ExprBinOp *expr, ASTdata data0, ASTdata data1 ) const
{
  ASTdata data;
  switch (expr->type()) {
    case PLUS:
      data.v = m_mathBuilder->CreateFAdd(data0.v, data1.v, "add");
      break;
    case MINUS:
      data.v = m_mathBuilder->CreateFSub(data0.v, data1.v, "sub");
      break;
    case MULT:
      data.v = m_mathBuilder->CreateFMul(data0.v, data1.v, "mult");
      break;
    case DIV:
      data.v = m_mathBuilder->CreateFDiv(data0.v, data1.v, "div");
      break;
    case POWER:
      data.v = callFunc( "pow", data0.v, data1.v );
      break;
    default:
      expr->error( "No such binary operator" );
  }

  return data;
}

ASTdata ASTVisitorLLVMIR::visit( const ExprFunction *expr, ASTdata data0 ) const
{
  ASTdata data;
  data.v = callFunc( Function::IDtoName(expr->id()), data0.v );
  return data;
}

ASTdata ASTVisitorLLVMIR::visit( const ExprFunction *expr, ASTdata data0, ASTdata data1 ) const
{
  ASTdata data;
  data.v = callFunc( Function::IDtoName(expr->id()), data0.v, data1.v );
  return data;
}

Value *ASTVisitorLLVMIR::callFunc( const string& name, Value *v0, Value *v1 ) const
{
  llvm::Function *func = m_mathModule->getFunction(name);

  vector<Value*> args;
  if (v0)
    args.push_back(v0);
  if (v1)
    args.push_back(v1);

  return m_mathBuilder->CreateCall(func, args, string("call_") + name);
}

#undef DBG
