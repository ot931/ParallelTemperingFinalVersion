cmake_minimum_required(VERSION 3.9)

project(metropolis VERSION 0.0.11)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(partsEngine)
add_subdirectory(argumentum-src)
add_subdirectory(inicpp)

add_custom_target(fix ALL
	COMMENT "fixing problem with argparse"

	COMMAND ${CMAKE_COMMAND}
	   -E copy
	   "${CMAKE_SOURCE_DIR}/argumentum-src/util/faketarget.cpp"
	   "${CMAKE_SOURCE_DIR}/util/faketarget.cpp"
)

find_package(OpenMP REQUIRED)

include_directories("partsEngine" "argumentum-src/include" "inicpp/include")

set(METROPOLIS_SRC
	main.cpp
	ConfigManager.cpp
	CorrelationCore.cpp 
	CorrelationPointCore.cpp 
	MagnetisationCore.cpp
	MagnetisationLengthCore.cpp
	misc.cpp
)

file(STRINGS examples/example.ini example_string_a)
list(JOIN example_string_a  "\\n"  example_string)

configure_file(defines.h.in defines.h)

add_executable(metropolis ${METROPOLIS_SRC})
target_link_libraries(metropolis partsEngine gmp gmpxx OpenMP::OpenMP_CXX argumentum inicpp gsl openblas)
target_include_directories(metropolis PUBLIC "${PROJECT_BINARY_DIR}")

add_executable(distanceAnalyser distanceAnalyser.cpp)
target_link_libraries(distanceAnalyser partsEngine argumentum)

add_executable(groundStater groundStater.cpp)
target_link_libraries(groundStater partsEngine argumentum)