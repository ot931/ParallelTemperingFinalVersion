#ifndef PARTARRAYTEST_H
#define PARTARRAYTEST_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "PartArray.h"
#include "config.h"
#include "random.h"
#include "ctime"

class PartArrayTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->set2D();
    }

    void EVsEComplete(){
        //тест вычисления энергии после создания системы
        PartArray *sys = new PartArray();
        sys->setInteractionRange(0.);
        for (int i=0;i<4;i++){
            for (int j=0;j<4;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                try{
                    sys->insert(temp);
                } catch(const string c){
                    cout<<c<<endl;
                }
            }
        }

        QCOMPARE(sys->E(), sys->EComplete());

        //после встряхивания состояния
        sys->state.randomize(10);
        QCOMPARE(sys->E(), sys->EComplete());

        //после обнуления конфигурации
        sys->state.reset();
        sys->state.randomize(10);
        QCOMPARE(sys->E(), sys->EComplete());

        //после полного обнуления конфигурации
        sys->state.hardReset();
        QCOMPARE(sys->E(), sys->EComplete());
        delete sys;
    }

    void EStepByStep(){
        //тест вычисления энергии после создания системы
        PartArray *sys = new PartArray();
        sys->setInteractionRange(0.);
        for (int i=0;i<4;i++){
            for (int j=0;j<4;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                try{
                    sys->insert(temp);
                } catch(const string c){
                    cout<<c<<endl;
                }
            }
        }

        for (int i=0; i<100; i++){
            sys->operator [](i%sys->count())->rotate(true);
            QCOMPARE(sys->E(), sys->EComplete());
        }

        for (int i=0; i<100; i++){
            sys->state.randomize();
            qFuzzyCompare(sys->E(), sys->EComplete());
        }

        delete sys;
    }


    void E(){
        int nums[100] = {
            -820, -537, -188, -706, -537,
            -354, -706, -1323, -649, 34,
            -88, -205, -416, 166, -656,
            -873, -278, -66, 753, 165,
            -66, 46, 165, -523, -907,
            -295, 53, -135, -746, -234,
            -585, -873, -649, -416, -88,
            -656, 34, 166, -205, -873,
            -578, 55, -88, -255, 55,
            587, -255, -523, -907, -746,
            53, -585, -295, -234, -135,
            -873, -1637, -1075, -747, -985,
            -1075, -613, -985, -1323, -537,
            -204, 109, -359, -263, -30,
            -417, -985, 34, 767, 609,
            542, 258, 890, 33, -135,
            -66, 196, 979, 441, 137,
            299, 383, -255, -295, 367,
            680, 542, -142, 420, 33,
            -205, -416, -134, 159, -359
        };

        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<3;i++)
            for (int j=0; j<3; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }

        for (int i=0;i<100;i++){
            QCOMPARE(int(floor(sys.E()*100)),nums[i]);
            sys.state.next();
        }
    }

    void EPartRotate(){
        PartArray sys;
        Part p1, p2, p3;
        p1.m = p3.m = Vect(0,1,0);
        p2.m = Vect(0,-1,0);
        p1.pos.setXYZ(0,0,0);
        p2.pos.setXYZ(1,0,0);
        p3.pos.setXYZ(2,0,0);
        sys.insert(p1);
        sys.insert(p2);
        sys.insert(p3);
        QCOMPARE(int(floor(sys.E()*1000)),-1875);
        int eOld = int(floor(sys.E()*1000));
        sys.parts[0]->rotate();
        QVERIFY(int(floor(sys.E()*1000)) != eOld);
    }

    void EByState(){
        int nums[100] = {
            -820, -537, -188, -706, -537,
            -354, -706, -1323, -649, 34,
            -88, -205, -416, 166, -656,
            -873, -278, -66, 753, 165,
            -66, 46, 165, -523, -907,
            -295, 53, -135, -746, -234,
            -585, -873, -649, -416, -88,
            -656, 34, 166, -205, -873,
            -578, 55, -88, -255, 55,
            587, -255, -523, -907, -746,
            53, -585, -295, -234, -135,
            -873, -1637, -1075, -747, -985,
            -1075, -613, -985, -1323, -537,
            -204, 109, -359, -263, -30,
            -417, -985, 34, 767, 609,
            542, 258, 890, 33, -135,
            -66, 196, 979, 441, 137,
            299, 383, -255, -295, 367,
            680, 542, -142, 420, 33,
            -205, -416, -134, 159, -359
        };

        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<3;i++)
            for (int j=0; j<3; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }

        sys.state.next();
        sys.state.next();
        StateMachineFree s(sys.count());
        for (int i=0;i<100;i++){
            QCOMPARE(int(floor(sys.E(s)*100)),nums[i]);
            s.next();
        }
    }

    void ENearestNeighbour(){
        double nums[10] = {
            -12, -10, -6, -12, -6,
            -4, -8, -14, -10, -8
        };

        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<3;i++)
            for (int j=0; j<3; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }

        for (int i=0;i<10;i++){
            qFuzzyCompare(sys.E(), nums[i]);
            sys.state.next();
        }
    }

    void ENearestNeighbourChangeOnFly(){
        double nums[10] = {
            -12, -10, -6, -12, -6,
            -4, -8, -14, -10, -8
        };

        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<3;i++)
            for (int j=0; j<3; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }
        sys.setInteractionRange(1.1);
        for (int i=0;i<10;i++){
            qFuzzyCompare(sys.E(), nums[i]);
            sys.state.next();
        }
    }

    void ENearestNeighbourFromState(){
        double nums[10] = {
            -12, -10, -6, -12, -6,
            -4, -8, -14, -10, -8
        };

        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<3;i++)
            for (int j=0; j<3; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }

        sys.state.next();
        sys.state.next();
        StateMachineFree s(sys.count());
        for (int i=0;i<10;i++){
            qFuzzyCompare(sys.E(s), nums[i]);
            s.next();
        }
    }

    void EFastUpdate(){
        PartArray sys;
        for (int i=0;i<3;i++)
            for (int j=0; j<3; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }

        for (int i=0;i<10;i++){
            sys.state.randomize();
            QCOMPARE(sys.E(), sys.EComplete());
        }
    }

    void eEmptySystem(){
        PartArray sys;
        qFuzzyIsNull(sys.E());
        sys.insert(Part());
        qFuzzyIsNull(sys.E());
    }

    void energyBenchmark1(){
        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<4;i++)
            for (int j=0; j<4; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }
        sys.state.fromString("0000000011111111");
        QBENCHMARK{sys.EComplete();}
    }

    void energyBenchmark2(){
        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<4;i++)
            for (int j=0; j<4; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }
        sys.state.fromString("0000000011111111");
        QBENCHMARK{sys.ECompleteFast();}
    }

    void energyBenchmark3(){
        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<4;i++)
            for (int j=0; j<4; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }
        sys.state.fromString("0000000011111111");
        QBENCHMARK{
            double E=0;
            for (unsigned i=0; i<sys.count(); i++)
                E+=sys.ECompleteOld(sys[i]);
        }
    }

    void energyBenchmark4(){
        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<4;i++)
            for (int j=0; j<4; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }
        sys.state.fromString("0000000011111111");
        QBENCHMARK{sys.E(); sys.changeState();}
    }

    void energyBenchmark5(){
        PartArray sys;
        sys.setInteractionRange(0.);
        for (int i=0;i<4;i++)
            for (int j=0; j<4; j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }
        sys.state.fromString("0000000011111111");
        StateMachineFree s; s.fromString("0000000011111111");
        QBENCHMARK{sys.E(s); sys.changeState();}
    }

    void constructor1(){
        for (int i=0;i<1000000;i++){
            PartArray sys1;
            sys1.insert(Part());
            sys1[0]->pos.setXYZ(1,2,3);
        }
    }

    void constructor2(){
        PartArray sys1;
        sys1.insert(Part());
        sys1[0]->pos.setXYZ(1,2,3);
        PartArray sys2 = PartArray(sys1);
        QCOMPARE(sys1.count(), sys2.count());
        QVERIFY(sys1[0]!=sys2[0]);
        QCOMPARE(sys1[0]->pos.x, sys2[0]->pos.x);
    }

    void testEquals(){
        PartArray sys1;
        sys1.insert(Part());
        sys1[0]->pos.setXYZ(1,2,3);
        sys1[0]->m.setXYZ(1,0,0);
        PartArray sys2 = PartArray(sys1);
        QVERIFY(sys1 == sys2);
        QCOMPARE(sys1[0]->pos.x, sys2[0]->pos.x);

        sys2.state.next();
        QVERIFY(sys1 == sys2);

        sys2.state.prev();
        sys1.E();
        QVERIFY(sys1 == sys2);

        sys2.clear();
        QVERIFY(!(sys1 == sys2));

        sys1.clear();
        QVERIFY(sys1 == sys2);
    }

    void testNonEquals(){
        PartArray sys1;
        sys1.insert(Part());
        sys1[0]->pos.setXYZ(1,2,3);
        sys1[0]->m.setXYZ(1,0,0);
        PartArray sys2 = PartArray(sys1);
        QVERIFY(!(sys1 != sys2));
        QCOMPARE(sys1[0]->pos.x, sys2[0]->pos.x);

        sys2.state.next();
        QVERIFY(!(sys1 != sys2));

        sys1.E();
        QVERIFY(!(sys1 != sys2));

        sys2.clear();
        QVERIFY(sys1 != sys2);

        sys1.clear();
        QVERIFY(!(sys1 != sys2));
    }

    void copy(){
        PartArray sys1;
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys1.insert(temp);
            }
        sys1.setMinstate(sys1.groundState());
        sys1.setMaxstate(sys1.maximalState());
        sys1.state.next();
        sys1.E();
        sys1.setInteractionRange(1.1);

        PartArray sys2 = PartArray(sys1);
        QVERIFY(sys1==sys2);
        QVERIFY(sys1.state == sys2.state);
        QCOMPARE(sys1.E(),sys2.E());
        QCOMPARE(sys1.interactionRange(), sys2.interactionRange());
    }

    void minstatetest(){
        PartArray sys1;
        for (int i=0;i<5;i++){
            Part temp;
            temp.pos.setXYZ(i,0,0);
            if (i%2==0)
                temp.m.y = 1;
            else
                temp.m.y = -1;
            sys1.insert(temp);
        }
        //проверяем наличие минимального состояния
        QCOMPARE((int)sys1.Minstate().size(),0);

        //проверяем правильно ли устанавливается
        StateMachineFree mstate = StateMachineFree();
        mstate.fromString("01010");
        sys1.setMinstate(mstate);
        QVERIFY(sys1.Minstate() == mstate);

        //сравниваем с настоящим GS
        sys1.setMinstate(sys1.groundState());
        mstate.fromString("11111");
        QVERIFY(sys1.Minstate() == mstate);
    }

    void maxstatetest(){
        PartArray sys1;
        for (int i=0;i<5;i++){
            Part temp;
            temp.pos.setXYZ(i,0,0);
            if (i%2==0)
                temp.m.y = 1;
            else
                temp.m.y = -1;
            sys1.insert(temp);
        }

        //проверяем наличие минимального состояния
        QCOMPARE((int)sys1.Maxstate().size(),0);

        //проверяем правильно ли устанавливается
        StateMachineFree mstate = StateMachineFree();
        mstate.fromString("10101");
        sys1.setMaxstate(mstate);
        QVERIFY(sys1.Maxstate() == mstate);

        //сравниваем с настоящим GS
        sys1.setMaxstate(sys1.maximalState());
        QVERIFY(sys1.Maxstate() == mstate);
    }

    void statetest(){
        PartArray sys1;
        for (int i=0;i<5;i++){
            Part temp;
            temp.pos.setXYZ(i,0,0);
            if (i%2==0)
                temp.m.y = 1;
            else
                temp.m.y = -1;
            sys1.insert(temp);
        }

        StateMachineFree mstate = StateMachineFree();
        QCOMPARE((int)sys1.State().size(),5);
        mstate.fromString("11111");
        QVERIFY(sys1.State()==mstate);
        mstate.fromString("01111");
        sys1[0]->rotate();
        QVERIFY(sys1.State()==mstate);
        sys1.setState(sys1.maximalState());
        mstate.fromString("10101");
        QVERIFY(sys1.State()==mstate);
    }

    void inserttest(){
        PartArray sys1,sys2;
        Part *p = new Part();
        sys1.insert(p);
        QVERIFY_EXCEPTION_THROWN(sys2.insert(p), string);
    }

    void saveload(){
        PartArray sys1, sys2;

        sys1.setInteractionRange(1.1);
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys1.insert(temp);
            }
        sys1.setMinstate(sys1.groundState());
        sys1.setMaxstate(sys1.maximalState());
        sys1.state.next();
        sys1.state.next();
        sys1.state.next();
        sys1.save("saveload.dat");
        sys2.load("saveload.dat");
        QVERIFY(sys1==sys2);
        QVERIFY(sys1.Minstate()==sys2.Minstate());
        QVERIFY(sys1.Maxstate()==sys2.Maxstate());
        QVERIFY(sys1.State()==sys2.State());
    }

    void chechIds(){
        PartArray a,b;
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                a.insert(temp);
            }

        a.setInteractionRange(1.1);
        for (unsigned i=0; i<a.size(); i++){
            QCOMPARE((unsigned)a[i]->Id(),i);
        }


        a.setInteractionRange(0);
        for (unsigned i=0; i<a.size(); i++){
            QCOMPARE((unsigned)a[i]->Id(),i);
        }

        a.save("sys"); b.load("sys"); //проверяем после сохранения
        for (unsigned i=0; i<b.size(); i++){
            QCOMPARE((unsigned)b[i]->Id(),i);
        }
    }

    void eFastTest(){
        PartArray a,b;
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                a.insert(temp);
            }
        b=a;

        a.state.next();

        for (int i=0;i<100;i++){
            a.state.randomize();
            b.state=a.state;
            QCOMPARE(a.E(),b.E());
        }
    }

    void energyAfterCopy(){
        PartArray a;
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                a.insert(temp);
            }
        double e1=a.E();
        PartArray b(a);
        QCOMPARE(e1,b.E());

        PartArray c;
        c = a;
        QCOMPARE(e1,c.E());
    }

    void stateAfterCopy(){
        PartArray a;
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                a.insert(temp);
            }
        a.state+=95;
        a.setMinstate(a.state);
        a.state+=95;
        a.setMaxstate(a.state);
        a.state+=95;

        PartArray b(a);
        QCOMPARE(a.Minstate(),b.Minstate());
        QCOMPARE(a.Maxstate(),b.Maxstate());
        QCOMPARE(a.State(),b.State());

        PartArray c;
        c = a;
        QCOMPARE(a.Minstate(),c.Minstate());
        QCOMPARE(a.Maxstate(),c.Maxstate());
        QCOMPARE(a.State(),c.State());
    }

    void energyAfterRandomize(){
        PartArray sys;
        for (int i=0;i<5;i++)
            for (int j=0;j<5;j++){
                Part temp;
                temp.pos.setXYZ(i,j,0);
                temp.m.setXYZ(0,1,0);
                sys.insert(temp);
            }

        QCOMPARE(sys.E(),sys.EComplete());

        sys.state.fromString("0100110010");
        int rotated = sys.state.randomize();
        QCOMPARE(sys.E(),sys.EComplete());

        sys.parts[rotated]->rotate();
        QCOMPARE(sys.E(),sys.EComplete());
    }

};

#endif // PARTARRAYTEST_H
