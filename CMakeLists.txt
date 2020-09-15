cmake_minimum_required(VERSION 3.15)

# Raytracer
PROJECT(Raytracer VERSION 1.0)

# Cpp settings
##############

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# INCLUDE
#########

# stb
set(STB src/ext/stb/)

# include
include_directories(${STB})

# main programm
###############
file(GLOB SRC_FILES 	
		src/core/*.cpp
		src/core/*.h)

add_executable(Raytracer ${SRC_FILES})

# testing
#########
file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/src/test/*.cpp)

add_executable(Raytracer_TestSuite ${TEST_FILES})