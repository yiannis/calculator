#include <sstream>

#include <QString>

#include "Interpreter.h"
#include "Test.h"

using namespace std;

void Test::initTestCase()
{
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
  engine.set( "x",  5.0F );
  engine.set( "y",  3.0F );
  engine.set( "t", -1.0F );

  QVERIFY( engine.run() );

  QCOMPARE( engine.result(), result );
}

QTEST_MAIN(Test)
