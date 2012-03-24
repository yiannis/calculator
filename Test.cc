#include <sstream>

#include <QString>

#include "Parser.h"
#include "Test.h"

using namespace std;

void Test::initTestCase()
{
  m_x = 5;
  m_y = 3;
}

void Test::parserTest_data()
{
  QTest::addColumn<QString>("function");
  QTest::addColumn<float>("result");

  QTest::newRow("parenthesis") << "(((x-10))^3)*y-(5+10)" << -390.0F;
  QTest::newRow("operator")    << "-3*x^2-8/2+(-2-5)"     << -86.0F;
  QTest::newRow("variables")   << "x/x-y^y+2*x"           << -17.0F;
}

void Test::parserTest()
{
  QFETCH(QString, function);
  QFETCH(float, result);

  istringstream input(function.toUtf8().constData());
  Lexer lexer(input, &m_x, &m_y);
  Parser parser(lexer.getTokens());

  QVERIFY( parser.OK() );

  QCOMPARE( result, parser.getAST()->result() );
}

QTEST_MAIN(Test)
