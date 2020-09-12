cmake_minimum_required(VERSION 3.15)

# Raytracer
PROJECT(Raytracer C CXX)

file(GLOB SRC_FILES 	
		src/core/*.cpp
		src/core/*.h)
add_executable(Raytracer ${SRC_FILES})

file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/src/test/*.cpp)

add_executable(Raytracer_TestSuite ${TEST_FILES})