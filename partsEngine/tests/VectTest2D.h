#ifndef VECTTEST2D_H
#define VECTTEST2D_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "Vect.h"
#include "config.h"
#include "ctime"

class VectTest2D : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->set2D();
        config::Instance()->srand(time(NULL));
    }
    void costructor(){
        Vect v;
        QCOMPARE(v.x,0.);
        QCOMPARE(v.y,0.);
        QCOMPARE(v.z,0.);

        Vect v1(1.1,2.2,3.3);
        QCOMPARE(v1.x,1.1);
        QCOMPARE(v1.y,2.2);
        QCOMPARE(v1.z,3.3);
    }
    void scalar(){
        Vect v1(1,0,0),v2(0,1,0);
        QCOMPARE(v1.scalar(v2),0.);
        v2.setXYZ(0.5,1,0);
        QVERIFY(v1.scalar(v2)==0.5);
        v2.setXYZ(-0.5,1,0);
        QVERIFY(v1.scalar(v2)==-0.5);
    }
    void space(){
        Vect v1(1,0,0),v2(0,1,0);
        Vect v3 = v1.radius(v2), v4 = v2.radius(v1);
        QVERIFY(v3.x==-1 && v3.y==1);
        QVERIFY(v4.x==1 && v4.y==-1);
    }
};

#endif // VECTTEST2D_H
