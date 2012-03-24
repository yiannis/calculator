#ifndef TEST_H
#define TEST_H

#include <QtTest>
#include <QObject>

class Test: public QObject
{
  Q_OBJECT

    private:
      float m_x, m_y;

    private slots:
      void initTestCase();

      void parserTest();
      void parserTest_data();
};

#endif //TEST_H
