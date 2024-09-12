cmake_minimum_required(VERSION 3.9)

project(partsEngine VERSION 0.0.1 DESCRIPTION "Library for creating and manipulating system of magnetic particles")

set (CMAKE_CXX_STANDARD 11)


set(SOURCES 
    Vect.cpp 
    config.cpp 
    Part.cpp 
    distributionLaw.cpp 
    statemachinebase.cpp 
    PartArray.cpp 
    StateMachine.cpp 
    StateMachineFree.cpp 
    squarespinicearray.cpp 
    squarespinicecell.cpp 
    wanglandau.cpp 
    honeycombspinicearray.cpp 
    honeycombspinicecell.cpp 
    squarelattice.cpp 
    amorphousarray.cpp 
    loadhelper.cpp 
    savehelper.cpp 
    squareisinglattice.cpp 
    clustermachine.cpp 
    dos2.cpp 
    dos3.cpp 
    random.cpp)

set(HEADERS 
    Vect.h 
    random.h 
    Part.h 
    statemachinebase.h 
    StateMachine.h 
    StateMachineFree.h 
    PartArray.h 
    config.h 
    squarespinicecell.h 
    squarespinicearray.h 
    honeycombspinicecell.h 
    honeycombspinicearray.h 
    squarelattice.h 
    distributionLaw.h 
    wanglandau.h 
    amorphousarray.h 
    loadhelper.h 
    savehelper.h 
    squareisinglattice.h 
    clustermachine.h 
    dos2.h 
    dos3.h 
    utils.h 
    brickworka.h 
    brickworkc.h)

set(OTHER_FILES 
    README.md 
    .gitignore)

option(BUILD_STATIC_LIB "Build static library" ON)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXECUTABLE "Build executable file for Debug purposes" OFF)

if(BUILD_STATIC_LIB)
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif()

target_link_libraries(${PROJECT_NAME})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_DEBUG)
    add_subdirectory(dbg)
endif()