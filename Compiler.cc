#include <llvm/Support/TargetSelect.h>
#include <llvm/LLVMContext.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/PassManager.h>

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
using llvm::TargetData;
using llvm::createBasicAliasAnalysisPass;
using llvm::createInstructionCombiningPass;
using llvm::createReassociatePass;
using llvm::createGVNPass;
using llvm::createCFGSimplificationPass;

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
  delete m_llvmIR;
}

void Compiler::set( const std::string& name, float value )
{
  ASTBuilder::set( name, value );

  if (m_constantIDs.size() > 0) // compiler has run
    m_constantsVector[m_constantIDs[name]] = value;
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

void Compiler::optimizeIR()
{
  llvm::FunctionPassManager fpm( m_mathModule );

  // Set up the optimizer pipeline.  Start with registering info about how the
  // target lays out data structures.
  fpm.add(new TargetData(*m_execEngine->getTargetData()));

  // Provide basic AliasAnalysis support for GVN.
  fpm.add(createBasicAliasAnalysisPass());

  // Do simple "peephole" optimizations and bit-twiddling optzns.
  fpm.add(createInstructionCombiningPass());

  // Reassociate expressions.
  fpm.add(createReassociatePass());

  // Eliminate Common SubExpressions.
  fpm.add(createGVNPass());

  // Simplify the control flow graph (deleting unreachable blocks, etc).
  fpm.add(createCFGSimplificationPass());

  fpm.doInitialization();

  // Optimize the main() function.
  fpm.run(*m_mainFunc);
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

void Compiler::initConstantsVector()
{
  const std::map<std::string,float>& constants = getConstantsTable();
  m_constantsVector.resize( constants.size() );
  for (IC i=constants.begin(); i!=constants.end(); i++)
    m_constantsVector[m_constantIDs[i->first]] = i->second;
}

void Compiler::compile()
{
  // Create the Module assembly code
  emmitIR();

  // Create the JIT.  This takes ownership of the module.
  m_execEngine = EngineBuilder(m_mathModule).create();

  // Optimize the Module assembly code
  optimizeIR();

  // JIT the function, returning a function pointer.
  void *pMain = m_execEngine->getPointerToFunction(m_mainFunc);

  // Cast it to the right type (takes no arguments, returns a double) so we
  // can call it as a native function.
  MainFPtr = (double (*)(double*))(intptr_t)pMain;

  // Create the input array to main()
  initConstantsVector();
}

float Compiler::result()
{
  return MainFPtr(&m_constantsVector[0]);
}
