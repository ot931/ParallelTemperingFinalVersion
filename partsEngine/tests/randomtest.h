#ifndef RANDOMTEST
#define RANDOMTEST

#include <QObject>
#include <QTest>
#include <QDebug>
#include "random.h"
#include "ctime"

class RandomTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
    }

    void standart(){
        Random::Instance()->standart();
        for (int i=0;i<10000;i++){
            int num = Random::Instance()->next();
            QVERIFY(num>=0 && num <= (int)Random::Instance()->max());
        }

        for (int i=0;i<10000;i++){
            int num = Random::Instance()->next(1000);
            QVERIFY(num>=0 && num <= 999);
        }



        for (int i=0;i<10000;i++){
            int num = Random::Instance()->next(101,1000);
            QVERIFY(num>=101 && num <= 999);
        }
    }

};

#endif // RANDOMTEST

