#ifndef HONEYCOMBSPINICETEST_H
#define HONEYCOMBSPINICETEST_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "honeycombspinicearray.h"
#include "config.h"
#include "ctime"

class HoneycombSpinIceTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->set2D();
        config::Instance()->srand(time(NULL));
        config::Instance()->partR = 0.5;
        config::Instance()->m = 1;
    }

    void dropHoneyCombTest(){
        HoneycombSpinIceArray *sys1 = new HoneycombSpinIceArray();
        sys1->dropHoneyComb(2,2,1);
        QCOMPARE(sys1->count(),(unsigned)19);
        QCOMPARE((int)sys1->cells.size(),4);

        sys1->dropHoneyComb(2,2,2);
        QCOMPARE(sys1->count(),(unsigned)19);
        QCOMPARE((int)sys1->cells.size(),4);
    }

    void copy(){
        HoneycombSpinIceArray *sys1,*sys2;
        sys1 = new HoneycombSpinIceArray();
        sys1->dropHoneyComb(2,2,1);
        sys2 = new HoneycombSpinIceArray(*sys1);
        QCOMPARE(sys2->count(),sys1->count());
        QCOMPARE(sys2->cells.size(),sys1->cells.size());
        QCOMPARE(sys1->EComplete(),sys2->EComplete());

        sys1->setMinstate(sys1->groundState());
        sys2->setMinstate(sys2->groundState());
        QCOMPARE(sys1->EComplete(),sys2->EComplete());

        sys1->setMaxstate(sys1->maximalState());
        sys2->setMaxstate(sys2->maximalState());
        QCOMPARE(sys1->EComplete(),sys2->EComplete());

        delete sys1;
        delete sys2;
    }
};

#endif // HONEYCOMBSPINICETEST_H

