#include <sstream>
#include <cmath>

#include <QString>

#include "Token.h"
#include "Function.h"
#include "Interpreter.h"
#include "Compiler.h"
#include "Test.h"

using namespace std;

/// "0.4*cos((2*pi/150)*t)*sin(x+y)/(x^2+y^2)"
double Test::func1d( double x, double y, double t )
{
  return 0.4*cos((2*M_PI/150)*t)*sin(x+y)/(pow(x,2)+pow(y,2));
}
/// "0.4*cos((2*pi/150)*t)*sin(x+y)/(x^2+y^2)"
float Test::func1f( float x, float y, float t )
{
  return 0.4*cos((2*M_PI/150)*t)*sin(x+y)/(pow(x,2)+pow(y,2));
}

void Test::lexerTest()
{
  istringstream source("\tsin ( pi/2.0 + x)\n * e");

  float x = 10.0F;
  Lexer lexer(&source);
  lexer.pushConstant( "x", &x );

  QCOMPARE( lexer.nextToken().m_data.id, Function::SIN );
  QCOMPARE( lexer.nextToken().m_type, LPAREN );
  QCOMPARE( lexer.nextToken().m_type, CONSTANT );
  QCOMPARE( lexer.nextToken().m_type, DIV );
  QCOMPARE( lexer.nextToken().m_data.value, 2.0F );
  QCOMPARE( lexer.nextToken().m_type, PLUS );
  QCOMPARE( lexer.nextToken().m_data.pValue, &x );
  QCOMPARE( lexer.nextToken().m_type, RPAREN );
  QCOMPARE( lexer.nextToken().m_type, MULT );
  QCOMPARE( lexer.nextToken().m_type, CONSTANT );
  QCOMPARE( lexer.nextToken().m_type, END );


  istringstream source_pi("pi");
  Lexer lexer_pi(&source_pi);

  Token pi = lexer_pi.nextToken();
  QCOMPARE( pi.m_type, CONSTANT );
  QCOMPARE( pi.m_name, string("pi") );
  QCOMPARE( *pi.m_data.pValue, Lexer::S_PI );
}

void Test::constantsTest()
{
  istringstream i_input("pi+e"), c_input("pi+e");

  Interpreter engine(&i_input);
  Compiler llvmc(&c_input);
  llvmc.compile();

  QCOMPARE( engine.result(), Lexer::S_PI+Lexer::S_E );
  QCOMPARE( llvmc.result(), Lexer::S_PI+Lexer::S_E );
}

void Test::parserTest_data()
{
  QTest::addColumn<QString>("function");
  QTest::addColumn<float>("result");

  QTest::newRow("parenthesis") << "(((x-10))^3)*y-(5+10)" << -390.0F;
  QTest::newRow("operator")    << "-3*x^2-8/2+(-2-5)"     << -86.0F;
  QTest::newRow("variables_xy")<< "x/x-y^y+2*x"           << -16.0F;
  QTest::newRow("variables_t") << "t/t-t^t+2*t"           << 0.0F;
  QTest::newRow("functions")   << "pow( sqrt( 4*x*log10(10^5) ), 2 )" << 100.0F;
  QTest::newRow("mixed")       << "(x-y)^pow(y-1,3/y)*(x*sqrt(25))"   << 100.0F;
}

void Test::parserTest()
{
  QFETCH(QString, function);
  QFETCH(float, result);

  istringstream input(function.toUtf8().constData());

  Interpreter engine(&input);
  engine.set( "x", 5.0F );
  engine.set( "y", 3.0F );
  engine.set( "t",-1.0F );

  QCOMPARE( engine.result(), result );
}

void Test::updateValuesTest()
{
  string func("x*y^z");
  istringstream i_input(func), c_input(func);

  Interpreter engine(&i_input);
  engine.set( "x", 4.0F );
  engine.set( "y", 2.0F );
  engine.set( "z", 2.0F );

  Compiler llvmc(&c_input);
  llvmc.set( "x", 4.0F );
  llvmc.set( "y", 2.0F );
  llvmc.set( "z", 2.0F );
  llvmc.compile();

  QCOMPARE( engine.result(), 16.0F );
  QCOMPARE( llvmc.result(),  16.0F );

  engine.set( "x", 3.0F );
  engine.set( "y", 3.0F );
  engine.set( "z", 3.0F );
  llvmc.set( "x", 3.0F );
  llvmc.set( "y", 3.0F );
  llvmc.set( "z", 3.0F );

  QCOMPARE( engine.result(), 81.0F );
  QCOMPARE( llvmc.result(), 81.0F );
}

void Test::updateCompilerValuesTest()
{
  string function("x+y+z");
  istringstream input(function);

  Compiler llvmc(&input);
  llvmc.set( "x", 0 );
  llvmc.set( "y", 0 );
  llvmc.set( "z", 0 );
  llvmc.compile();

  for (float x=0; x<100; x++) {
    llvmc.set( "x", x );

    QCOMPARE( llvmc.result(), x );
  }
  llvmc.set( "x", 0 );

  for (float y=0; y<100; y++) {
    llvmc.set( "y", y );

    QCOMPARE( llvmc.result(), y );
  }
  llvmc.set( "y", 0 );

  for (float z=0; z<100; z++) {
    llvmc.set( "z", z );

    QCOMPARE( llvmc.result(), z );
  }
  llvmc.set( "z", 0 );

  for (float x=0; x<100; x++)
    for (float y=0; y<100; y++)
      for (float z=0; z<100; z++) {
        llvmc.set( "x", x );
        llvmc.set( "y", y );
        llvmc.set( "z", z );

        QCOMPARE( llvmc.result(), x+y+z );
      }
}

void Test::CompilerTest_data()
{
  parserTest_data();
}

void Test::CompilerTest()
{
  QFETCH(QString, function);
  QFETCH(float, result);

  istringstream input(function.toUtf8().constData());

  Compiler llvmMath(&input);
  llvmMath.set( "x", 5.0F );
  llvmMath.set( "y", 3.0F );
  llvmMath.set( "t",-1.0F );
  llvmMath.compile();

  QCOMPARE( llvmMath.result(), result );
}

void Test::CompilerVsInterpreterBenchmark_data()
{
  QTest::addColumn<bool>("useCompiler");
  QTest::newRow("Compiler") << true;
  QTest::newRow("Interpreter") << false;
}

void Test::CompilerVsInterpreterBenchmark()
{
  QFETCH(bool, useCompiler);

  string function("(x-y)^pow(y-1,3/y)*(x*sqrt(25))");
  istringstream input(function);
  float result;

  if (useCompiler) {
    Compiler llvmc(&input);
    llvmc.set( "x", 5.0F );
    llvmc.set( "y", 3.0F );
    llvmc.set( "t",-1.0F );
    llvmc.compile();

    QBENCHMARK { result = llvmc.result(); }
  } else {
    Interpreter engine(&input);
    engine.set( "x", 5.0F );
    engine.set( "y", 3.0F );
    engine.set( "t",-1.0F );

    QBENCHMARK { result = engine.result(); }
  }

  QCOMPARE( result, 100.0F );
}

void Test::fullValuesCompilerTest()
{
  istringstream input(func1_str);

  Compiler llvmc(&input);
  llvmc.set( "x", 0.0F );
  llvmc.set( "y", 0.0F );
  llvmc.set( "t", 0.0F );
  llvmc.compile();

  for (float x=-5.0F; x<5.0F; x+=0.2F)
    for (float y=-5.0F; y<5.0F; y+=0.2F)
      for (float t=0.0F; t<100.0F; t++) {
        llvmc.set( "x", x );
        llvmc.set( "y", y );
        llvmc.set( "t", t );

//        QCOMPARE( func1d( x, y, t ), (double)llvmc.result() );
        double native_result = func1d( x, y, t ),
               llvm_result = llvmc.result(),
               diff = abs(native_result - llvm_result);
        QVERIFY( abs(diff/native_result) < 0.001 );
      }
  cerr << "\n";
}

void Test::fullValuesInterpreterTest()
{
  istringstream input(func1_str);

  Interpreter engine(&input);
  engine.set( "x", 0.0F );
  engine.set( "y", 0.0F );
  engine.set( "t", 0.0F );

  for (float x=-5.0F; x<5.0F; x+=0.2F)
    for (float y=-5.0F; y<5.0F; y+=0.2F)
      for (float t=0.0F; t<100.0F; t++) {
        engine.set( "x", x );
        engine.set( "y", y );
        engine.set( "t", t );

        QCOMPARE( func1f( x, y, t ), engine.result() );
      }
}

std::string Test::func1_str = "0.4*cos((2*pi/150)*t)*sin(x+y)/(x^2+y^2)";

QTEST_MAIN(Test)
