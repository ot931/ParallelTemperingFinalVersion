#ifndef STATEMACHINEFREETEST_H
#define STATEMACHINEFREETEST_H

#include <QObject>
#include <QTest>
#include <QDebug>
#include "PartArray.h"
#include "StateMachine.h"
#include "StateMachineFree.h"
#include <string>
#include <ctime>

class StateMachineFreeTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase(){
        config::Instance()->srand(time(NULL));
        config::Instance()->set2D();
    }

    void constructor1(){
        StateMachineFree s;
        QVERIFY(s.size()==0);
    }

    void constructor2(){
        StateMachineFree s1(10);
        QVERIFY(s1.size()==10);
        QCOMPARE(s1[5],false);
    }

    void constructor3(){
        PartArray sys;
        for (int i=0; i<5; i++)
            sys.insert(Part());
        sys.parts[3]->rotate();
        StateMachineFree s2(sys.state);

        QVERIFY(s2.size()==5);

        QCOMPARE(s2[0],false);
        QCOMPARE(s2[3],true);
    }

    void reset(){
        StateMachineFree s1(5);
        s1+=5;
        s1.reset();
        QCOMPARE(s1.isFirst(),true);
    }

    void setLast(){
        StateMachineFree s1(2);
        s1.setLast();

        QVERIFY(s1[0]);
        QVERIFY(s1[1]);
    }

    void randonize(){
        StateMachineFree s1(100);

        QVERIFY(s1.randomize()<100);
        QVERIFY(!s1.isFirst());

        s1.reset();

        QCOMPARE(s1.randomize(2),(unsigned)2);
        QVERIFY(!s1.isFirst());
    }

    void isFirst(){
        StateMachineFree s1(100);
        s1+=10;
        QVERIFY(!s1.isFirst());
        s1.reset();
        QVERIFY(s1.isFirst());
    }

    void isLast(){
        StateMachineFree s1(10);
        QVERIFY(!s1.isLast());
        s1+=100;
        QVERIFY(!s1.isLast());
        s1+=(1024-101);
        QVERIFY(s1.isLast());
    }

    void isHalfLast(){
        StateMachineFree s1(10);
        QVERIFY(!s1.isHalfLast());
        s1+=100;
        QVERIFY(!s1.isHalfLast());
        s1+=(512-101);
        QVERIFY(s1.isHalfLast());
    }

    void next(){
        StateMachineFree s1(3);

        QCOMPARE(s1.next(),true);
        QCOMPARE(s1[0],true);
        QCOMPARE(s1[1],false);
        QCOMPARE(s1[2],false);


        QCOMPARE(s1.next(),true);
        QCOMPARE(s1[0],false);
        QCOMPARE(s1[1],true);
        QCOMPARE(s1[2],false);


        int i=2;
        while (s1.next()) i++;

        QCOMPARE(i,7);
        QVERIFY(s1.isFirst());

        StateMachineFree s2;
        QVERIFY(!s2.next());
    }

    void halfNext(){
        StateMachineFree s1(3);

        QCOMPARE(s1.halfNext(),true);
        QCOMPARE(s1[0],true);
        QCOMPARE(s1[1],false);
        QCOMPARE(s1[2],false);


        QCOMPARE(s1.halfNext(),true);
        QCOMPARE(s1[0],false);
        QCOMPARE(s1[1],true);
        QCOMPARE(s1[2],false);


        int i=2;
        while (s1.halfNext()) i++;

        QCOMPARE(i,3);
        QVERIFY(s1.isFirst());
    }

    void prev(){
        StateMachineFree s1(3);

        QVERIFY(!s1.prev());
        QCOMPARE(s1[0],true);
        QCOMPARE(s1[1],true);
        QCOMPARE(s1[2],true);

        QVERIFY(s1.prev());
        QCOMPARE(s1[0],false);
        QCOMPARE(s1[1],true);
        QCOMPARE(s1[2],true);

        int i=2;
        while (s1.prev()) i++;
        QCOMPARE(i,8);
        QVERIFY(s1.isLast());

        StateMachineFree s2;
        QVERIFY(!s2.prev());
    }

    void halfPrev(){
        StateMachineFree s1(3);

        QVERIFY(!s1.halfPrev());
        QCOMPARE(s1[0],true);
        QCOMPARE(s1[1],true);
        QCOMPARE(s1[2],false);

        QVERIFY(s1.halfPrev());
        QCOMPARE(s1[0],false);
        QCOMPARE(s1[1],true);
        QCOMPARE(s1[2],false);

        int i=2;
        while (s1.halfPrev()) i++;
        QCOMPARE(i,4);
        QVERIFY(s1.isHalfLast());
    }

    void plusplus(){
        StateMachineFree s1(3),s2(3);
        s1.next();s2++;
        s1.next();s2++;
        s1.next();s2++;
        QVERIFY(s1==s2);

        int i=3;
        while (s2++) i++;
        QCOMPARE(i,7);
        QVERIFY(s2.isFirst());
    }

    void minusminus(){
        StateMachineFree s1(3),s2(3);
        s1.prev();s2--;
        s1.prev();s2--;
        s1.prev();s2--;
        QVERIFY(s1==s2);

        int i=3;
        while (s2--) i++;
        QCOMPARE(i,8);
        QVERIFY(s2.isLast());
    }

    void toString(){
        StateMachineFree s1(4);
        s1.halfPrev();
        QVERIFY(s1.toString().compare("1110")==0);
        s1.halfPrev();
        s1.halfPrev();
        QVERIFY(s1.toString().compare("1010")==0);
    }

    void fromString(){
        StateMachineFree s1(10),s2;
        s1+=1022;
        s2.fromString(s1.toString());
        QVERIFY(s2==s1);
    }

    void getelement(){
        StateMachineFree s1(10);
        s1+=1022;
        QVERIFY(s1[7]);
    }

    void size(){
        StateMachineFree s1(10);
        QCOMPARE((int)s1.size(),10);
        s1.resize(15);
        QCOMPARE((int)s1.size(),15);
    }

    void equally(){
        StateMachineFree s1(10),s2;
        s1+=1022;
        s2=s1;
        QVERIFY(&s1!=&s2);
        QCOMPARE(s2.size(),s1.size());
        s2+=2;
        QVERIFY(s2.isFirst());
    }

    void equals(){
        StateMachineFree s1(5),s2(5);
        s1+=10; s2+=5;
        QVERIFY(!(s1==s2));
        s2+=5;
        QVERIFY(s1==s2);
        StateMachineFree s3(6);
        s3+=10;
        QVERIFY(!(s1==s3));
    }

    void randomTest(){
        StateMachineFree s(16);

        unsigned c[16];
        for (unsigned i=0;i<16;i++){
            c[i]=0;
        }

        unsigned tempNum;
        for (unsigned i=0;i<10000000;i++){
            tempNum=s.randomize();
            c[tempNum]++;
        }

        for (unsigned i=0;i<16;i++){
            QVERIFY(c[i]>0);
        }
    }
};

#endif // STATEMACHINEFREETEST_H

