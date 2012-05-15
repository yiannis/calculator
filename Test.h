#ifndef TEST_H
#define TEST_H

#include <QtTest>
#include <QObject>

class Test: public QObject
{
  Q_OBJECT

    private:
      static std::string func1_str;

      double func1d( double x, double y, double t );
      float func1f( float x, float y, float t );

    private slots:
      void lexerTest();

      void constantsTest();

      void parserTest();
      void parserTest_data();

      void updateValuesTest();
      void updateCompilerValuesTest();

      void CompilerTest();
      void CompilerTest_data();

      void CompilerVsInterpreterBenchmark();
      void CompilerVsInterpreterBenchmark_data();

      void fullValuesCompilerTest();
      void fullValuesInterpreterTest();
};

#endif //TEST_H
