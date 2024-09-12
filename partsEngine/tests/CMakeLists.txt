cmake_minimum_required(VERSION 3.9)

project(partsEngineTest LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Test REQUIRED)
find_package(Qt5Core)

enable_testing(true)

include_directories("..")
link_directories("..")

set(SOURCES
    main.cpp
    dos2test.h
    honeycombspinicetest.h
    PartArrayTest.h
    parttest.h
    squarespinicetest.h
    statemachinefreetest.h
    StateMachineTest.h
    VectTest2D.h
    )

add_executable(partsEngineTest ${SOURCES})
add_test(NAME partsEngineTest COMMAND partsEngineTest)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Test partsEngine)