#-------------------------------------------------
#
# Project created by QtCreator 2014-07-15T10:13:48
#
#-------------------------------------------------

QT       += core testlib

QT       -= gui

TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../savehelper.cpp \
    ../squarelattice.cpp \
    ../squarespinicearray.cpp \
    ../squarespinicecell.cpp \
    ../StateMachine.cpp \
    ../statemachinebase.cpp \
    ../StateMachineFree.cpp \
    ../Vect.cpp \
    ../wanglandau.cpp \
    ../random.cpp \
    ../Part.cpp \
    ../PartArray.cpp \
    ../amorphousarray.cpp \
    ../config.cpp \
    ../distributionLaw.cpp \
    ../honeycombspinicearray.cpp \
    ../honeycombspinicecell.cpp \
    ../loadhelper.cpp \
    ../dos2.cpp \
    ../dos3.cpp

HEADERS += \
    VectTest2D.h \
    StateMachineTest.h \
    statemachinefreetest.h \
    squarespinicetest.h \
    randomtest.h \
    honeycombspinicetest.h \
    PartArrayTest.h \
    parttest.h \
    ../savehelper.h \
    ../squarelattice.h \
    ../squarespinicearray.h \
    ../squarespinicecell.h \
    ../StateMachine.h \
    ../statemachinebase.h \
    ../StateMachineFree.h \
    ../Vect.h \
    ../wanglandau.h \
    ../random.h \
    ../Part.h \
    ../PartArray.h \
    ../amorphousarray.h \
    ../config.h \
    ../distributionLaw.h \
    ../honeycombspinicearray.h \
    ../honeycombspinicecell.h \
    ../loadhelper.h \
    dos2test.h \
    ../dos2.h \
    ../dos3.h

INCLUDEPATH += ../

CONFIG += c++11
