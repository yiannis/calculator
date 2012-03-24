#ifndef TEST_H
#define TEST_H

#include <QtTest>
#include <QObject>

class Test: public QObject
{
  Q_OBJECT

    private slots:
      void parenthesisTest();
      void operatorsTest();
      void variablesTest();
};

#endif //TEST_H
