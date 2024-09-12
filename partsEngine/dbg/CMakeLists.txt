cmake_minimum_required(VERSION 3.9)

project(partsEngineDbg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Test REQUIRED)
find_package(Qt5Core)

include_directories("..")
link_directories("..")

set(SOURCES
    main.cpp
    )

add_executable(partsEngineDbg ${SOURCES})
target_link_libraries(${PROJECT_NAME} partsEngine)