#include <sstream>
#include <iostream>

#include <QString>

#include "Parser.h"
#include "Test.h"

using namespace std;

void Test::initTestCase()
{
  m_x = 5;
  m_y = 3;
  m_t = -1;
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

  cerr << function.toUtf8().constData() << endl;

  istringstream input(function.toUtf8().constData());

  Lexer lexer(&input);
  lexer.pushConstant( string("x"), &m_x );
  lexer.pushConstant( string("y"), &m_y );
  lexer.pushConstant( string("t"), &m_t );

  Parser parser(&lexer);

  QVERIFY( parser.AST() );

  QCOMPARE( parser.AST()->result(), result );
}

QTEST_MAIN(Test)
