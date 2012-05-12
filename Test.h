#ifndef TEST_H
#define TEST_H

#include <QtTest>
#include <QObject>

class Test: public QObject
{
  Q_OBJECT

    private:

    private slots:
      void lexerTest();

      void parserTest();
      void parserTest_data();

      void updateValuesTest();

      void CompilerTest();
      void CompilerTest_data();
};

#endif //TEST_H
