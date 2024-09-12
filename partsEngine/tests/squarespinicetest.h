#ifndef SQUARESPINICETEST
#define SQUARESPINICETEST

#include <QObject>
#include <QTest>
#include <QDebug>
#include "squarespinicearray.h"
#include "config.h"
#include "ctime"

class SquareSpinIceTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->set2D();
        config::Instance()->srand(time(NULL));
        config::Instance()->partR = 0.5;
        config::Instance()->m = 1;
    }

    void cellTypes(){
        SquareSpinIceArray *sys1;
        sys1 = new SquareSpinIceArray();
        sys1->dropSpinIce(1,1);
        QCOMPARE((int)sys1->cells.size(),1);
        sys1->clear();

        sys1->dropSpinIce(2,2);
        QCOMPARE((int)sys1->cells.size(),4);

        sys1->setMaxstate(sys1->maximalState());
        for (int i=0;i<4;i++)
            QCOMPARE(sys1->cells[i]->type(),3);

        sys1->cells[0]->top->rotate();
        QCOMPARE(sys1->cells[0]->type(),2);


        sys1->setMinstate(sys1->groundState());
        for (int i=0;i<4;i++)
            QCOMPARE(sys1->cells[i]->type(),0);
    }

    void copy(){
        SquareSpinIceArray *sys1,*sys2;
        sys1 = new SquareSpinIceArray();
        sys1->dropSpinIce(3,3);
        sys2 = new SquareSpinIceArray(*sys1);
        QCOMPARE(sys2->count(),sys1->count());
        QCOMPARE(sys1->EComplete(),sys2->EComplete());

        sys1->setMinstate(sys1->groundState());
        sys2->setMinstate(sys2->groundState());
        QCOMPARE(sys1->EComplete(),sys2->EComplete());

        sys1->setMaxstate(sys1->maximalState());
        sys2->setMaxstate(sys2->maximalState());
        QCOMPARE(sys1->EComplete(),sys2->EComplete());
        delete sys1; delete sys2;
    }

    void dropSpinIce(){
        SquareSpinIceArray a;
        a.dropSpinIce(2,2,3);

        QCOMPARE(a.parts[0]->pos.x,1.5);
        QCOMPARE(a.parts[0]->pos.y,3.);

        QCOMPARE(a.parts[1]->pos.x,1.5);
        QCOMPARE(a.parts[1]->pos.y,0.);

        QCOMPARE(a.parts[2]->pos.x,0.);
        QCOMPARE(a.parts[2]->pos.y,1.5);

        QCOMPARE(a.parts[3]->pos.x,3.);
        QCOMPARE(a.parts[3]->pos.y,1.5);

        QCOMPARE(a.count(),(unsigned)12);
    }

    void chechIds(){
        SquareSpinIceArray a,b;
        a.dropSpinIce(10,10,1);

        a.setInteractionRange(0.8); //проверяем для короткодействия
        for (unsigned i=0; i<a.size(); i++){
            QCOMPARE((unsigned)a[i]->Id(),i);
        }

        a.setInteractionRange(0); //проверяем для дальнодействия
        for (unsigned i=0; i<a.size(); i++){
            QCOMPARE((unsigned)a[i]->Id(),i);
        }

        a.save("sys"); b.load("sys"); //проверяем после сохранения
        for (unsigned i=0; i<b.size(); i++){
            QCOMPARE((unsigned)b[i]->Id(),i);
        }
    }
};


#endif // SQUARESPINICETEST

