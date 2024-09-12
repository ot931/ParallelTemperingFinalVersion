#include <QCoreApplication>

#include "VectTest2D.h"
#include "PartArrayTest.h"
#include "StateMachineTest.h"
#include "statemachinefreetest.h"
#include "squarespinicetest.h"
#include "honeycombspinicetest.h"
//#include "randomtest.h"
#include "dos2test.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "C");
    int num=0;
    num+=QTest::qExec(new VectTest2D, argc, argv);
    num+=QTest::qExec(new PartArrayTest, argc, argv);
    num+=QTest::qExec(new SquareSpinIceTest, argc, argv);
    num+=QTest::qExec(new HoneycombSpinIceTest, argc, argv);
    num+=QTest::qExec(new StateMachineTest, argc, argv);
    num+=QTest::qExec(new StateMachineFreeTest, argc, argv);
    //num+=QTest::qExec(new RandomTest, argc, argv);
    num+=QTest::qExec(new Dos2Test, argc, argv);

    qWarning("Have %d errors!",num);

    return a.exec();
}
