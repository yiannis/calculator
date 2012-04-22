#ifndef TEST_H
#define TEST_H

#include <QtTest>
#include <QObject>

#include "Executor.h"

class Test: public QObject
{
  Q_OBJECT

    private:
      float m_x, m_y, m_t;

    private slots:
      void initTestCase();

      void parserTest();
      void parserTest_data();
};

#endif //TEST_H
