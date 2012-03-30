#ifndef TEST_H
#define TEST_H

#include <QtTest>
#include <QObject>

#include "Executor.h"

class Test: public QObject
{
  Q_OBJECT

    private:
      Executor m_code;

    private slots:
      void initTestCase();

      void parserTest();
      void parserTest_data();
};

#endif //TEST_H
