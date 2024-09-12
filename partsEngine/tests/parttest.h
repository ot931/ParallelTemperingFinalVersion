#ifndef PARTTEST_H
#define PARTTEST_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "Part.h"
#include "PartArray.h"

class PartTest : public QObject
{
    Q_OBJECT

private slots:
    void constructor(){
        Part *p = new Part();
        QVERIFY(p->m == Vect(0,0,0));
        QVERIFY(p->pos == Vect(0,0,0));
        QCOMPARE(p->Id(),(long)-1);
        delete p;
    }

    void constructor2(){
        Part p1;
        p1.pos.setXYZ(1.,2.,3.);
        p1.m.setXYZ(4,5,6);
        p1.rotate();
        Part p2 = Part(p1);
        QVERIFY(p1==p2);
    }

    void equals(){
        //проверяем равенство состояния
        Part p1;
        p1.pos.setXYZ(1.,2.,3.);
        p1.m.setXYZ(4,5,6);
        Part p2;
        p2.pos.setXYZ(1.,2.,3.);
        p2.m.setXYZ(-4,-5,-6);
        p2.rotate();
        QVERIFY(!(p1==p2));

        p2.rotate();
        p2.m*=-1;
        QVERIFY(p1==p2);


        //проверяем равенство намагниченности
        p2.m.x = 5;
        QVERIFY(!(p1==p2));

        p2.m.x = 4;
        //проверяем равенство позиции
        p2.pos.x=2;
        QVERIFY(!(p1==p2));
        p2.pos.x = 1;
    }

    void nonequals(){
        Part p1, p2;
        p1.pos.setXYZ(1.,2.,3.);
        p1.m.setXYZ(4,5,6);
        p2=p1;


        QVERIFY(!(p1!=p2));
        p2.rotate();
        QVERIFY(!(p1!=p2));
        p2.m.x=-2;
        QVERIFY(p1!=p2);
    }

    void interact(){
        Part p1, p2;
        p2.pos.x =1;
        p2.m = p1.m = Vect(0,1,0);
        QVERIFY(p1.interact(p2.pos)==p2.interact(p1.pos));
        QCOMPARE(p1.interact(p2.pos).length(),11.);
    }

    void rotate(){
        Part p1, p2;
        p1.m = Vect(1,2,3);
        p2 = p1;
        p1.rotate();
        QVERIFY(p1.m == p2.m*-1);
        QCOMPARE(p1.state, !p2.state);
        p1.rotate();
        QVERIFY(p1.m == p2.m);
        QCOMPARE(p1.state, p2.state);
    }

    void id(){
        Part p;
        PartArray sys;
        QCOMPARE(p.Id(),(long)-1);
        sys.insert(p);
        QCOMPARE(p.Id(),(long)0);
    }
};

#endif // PARTTEST_H
