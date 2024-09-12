#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T10:01:07
# For MPI version needs to write `qmake PartsEngine.pro "MODES=mpi"` in console.
#
#-------------------------------------------------

CONFIG(release,debug|release){
    TARGET = PartsEngine
    DEFINES += QT_NO_DEBUG_OUTPUT
}
CONFIG(debug,debug|release){
    TARGET = dPartsEngine
}

QT       += core

SUBDIRS += tests

QT       -= gui

SOURCES += Vect.cpp \
    config.cpp \
    Part.cpp \
    distributionLaw.cpp \
    statemachinebase.cpp \
    PartArray.cpp \
    StateMachine.cpp \
    StateMachineFree.cpp \
    squarespinicearray.cpp \
    squarespinicecell.cpp \
    wanglandau.cpp \
    honeycombspinicearray.cpp \
    honeycombspinicecell.cpp \
    squarelattice.cpp \
    amorphousarray.cpp \
    loadhelper.cpp \
    savehelper.cpp \
    squareisinglattice.cpp \
    clustermachine.cpp \
    dos2.cpp \
    dos3.cpp \
    random.cpp

HEADERS += \
    Vect.h \
    random.h \
    Part.h \
    statemachinebase.h \
    StateMachine.h \
    StateMachineFree.h \
    PartArray.h \
    config.h \
    squarespinicecell.h \
    squarespinicearray.h \
    honeycombspinicecell.h \
    honeycombspinicearray.h \
    squarelattice.h \
    distributionLaw.h \
    wanglandau.h \
    amorphousarray.h \
    loadhelper.h \
    savehelper.h \
    squareisinglattice.h \
    clustermachine.h \
    dos2.h \
    dos3.h \
    utils.h \
    brickworka.h \
    brickworkc.h


TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD

OTHER_FILES += \
    README.md \
    .gitignore

CONFIG += c++11

DEFINES += _USE_MATH_DEFINES
