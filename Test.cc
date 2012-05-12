#include <sstream>

#include <QString>

#include "Token.h"
#include "Function.h"
#include "Interpreter.h"
#include "Compiler.h"
#include "Test.h"

using namespace std;

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
  engine["x"] =  5.0F;
  engine["y"] =  3.0F;
  engine["t"] = -1.0F;

  QCOMPARE( engine.result(), result );
}

void Test::updateValuesTest()
{
  istringstream input("x*y^z");

  Interpreter engine(&input);
  engine["x"] =  4.0F;
  engine["y"] =  2.0F;
  engine["z"] =  2.0F;

  QCOMPARE( engine.result(), 16.0F );

  engine["x"] =  3.0F;
  engine["y"] =  3.0F;
  engine["z"] =  3.0F;

  QCOMPARE( engine.result(), 81.0F );
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
  llvmMath["x"] =  5.0F;
  llvmMath["y"] =  3.0F;
  llvmMath["t"] = -1.0F;
  llvmMath.emmitIR();

  QCOMPARE( llvmMath.result(), result );
}

QTEST_MAIN(Test)
