#include <llvm/Support/TargetSelect.h>
#include <llvm/LLVMContext.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>

#include "Compiler.h"

using namespace std;
using llvm::getGlobalContext;
using llvm::InitializeNativeTarget;
using llvm::Module;
using llvm::IRBuilder;
using llvm::LLVMContext;
using llvm::Type;
using llvm::FunctionType;
using llvm::BasicBlock;
using llvm::Value;
using llvm::verifyFunction;
using llvm::ExecutionEngine;
using llvm::EngineBuilder;

Compiler::Compiler(istream *input) :
  ASTBuilder(input)
{
  InitializeNativeTarget(); //FIXME

  m_mathModule = new Module( "math jit", getGlobalContext() );
  m_mathBuilder = new IRBuilder<>( getGlobalContext() );
  m_llvmIR = new ASTVisitorLLVMIR( m_mathModule, m_mathBuilder );

  initMathFunctions();
}

Compiler::~Compiler()
{
  delete m_mathModule;
  delete m_mathBuilder;
}

void Compiler::emmitIR()
{
  initConstantIDs();
  m_llvmIR->setConstantIDs( &m_constantIDs );

  LLVMContext &Context = getGlobalContext();

  // Create a 'double main(double*)' prototype
  vector<Type*> main_args;
  main_args.push_back( Type::getDoublePtrTy(Context) );
  FunctionType *mainType = FunctionType::get( Type::getDoubleTy(Context), main_args, false );

  // Create the 'double main(double* argv)' function definition inside m_mathModule
  m_mainFunc = llvm::Function::Create( mainType, llvm::Function::ExternalLinkage, "main", m_mathModule );
  m_mainFunc->arg_begin()->setName("argv");

  // Create a new basic block to start insertion into 'main()'
  BasicBlock *mainBlock = BasicBlock::Create( Context, "entry_point", m_mainFunc );
  m_mathBuilder->SetInsertPoint(mainBlock);

  // Fill the block with the experssion from the AST
  Value *res = visitorResult( m_llvmIR ).v;

  // Set the main() function's return value
  m_mathBuilder->CreateRet( res );

  // Validate the generated code, checking for consistency.
  verifyFunction( *m_mainFunc );
}

void Compiler::printIR()
{
  m_mathModule->dump();
}

void Compiler::initMathFunctions()
{
  LLVMContext &Context = getGlobalContext();

  for (int id=0; id<Function::numFunctions(); id++) {
    // Create a 'double func(double)' prototype
    vector<Type*> double_args;
    if (Function::numArguments[id] >= 1)
      double_args.push_back( Type::getDoubleTy(Context) );
    if (Function::numArguments[id] >= 2)
      double_args.push_back( Type::getDoubleTy(Context) );
    FunctionType *ftype = FunctionType::get( Type::getDoubleTy(Context), double_args, false );

    // Create the 'double func(double)' function definition inside the m_mathModule
    llvm::Function *func = llvm::Function::Create(
        ftype,
        llvm::Function::ExternalLinkage,
        Function::IDtoName( static_cast<Function::ID>(id) ),
        m_mathModule
    );
  }
}

void Compiler::initConstantIDs()
{
    int idx = 0;
    const std::map<std::string,float>& constants = getConstantsTable();
    for (IC i=constants.begin(); i!=constants.end(); i++, idx++)
      m_constantIDs[i->first] = idx;
}

float Compiler::result()
{
  // Create the JIT.  This takes ownership of the module.
  ExecutionEngine *execEngine = EngineBuilder(m_mathModule).create();

  // JIT the function, returning a function pointer.
  void *pMain = execEngine->getPointerToFunction(m_mainFunc);

  // Cast it to the right type (takes no arguments, returns a double) so we
  // can call it as a native function.
  double (*MainFPtr)(double*) = (double (*)(double*))(intptr_t)pMain;

  const std::map<std::string,float>& constants = getConstantsTable();
  vector<double> constantsVector( constants.size() );
  for (IC i=constants.begin(); i!=constants.end(); i++) {
    constantsVector[m_constantIDs[i->first]] = i->second;
  }

  return MainFPtr(&constantsVector[0]);
}
