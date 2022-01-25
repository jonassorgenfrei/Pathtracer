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

# boost
set(Boost_NO_SYSTEM_PATHS true)
set (Boost_USE_STATIC_LIBS OFF CACHE BOOL "use static libraries from Boost")
set (Boost_USE_MULTITHREADED ON)

find_package(Boost REQUIRED COMPONENTS program_options REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

# include
include_directories(${STB})

# main programm
###############
file(GLOB SRC_FILES 	
		src/core/*.cpp
		src/core/*.h)

add_executable(Raytracer ${SRC_FILES})

# linking
#########
target_link_libraries(Raytracer Boost::program_options)

# testing
#########
file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/src/test/*.cpp)

add_executable(Raytracer_TestSuite ${TEST_FILES})

if (WIN32)
  # disable autolinking in boost
  add_definitions( -DBOOST_ALL_NO_LIB )

  # force all boost libraries to dynamic link (we already disabled
  # autolinking, so I don't know why we need this, but we do!)
  add_definitions( -DBOOST_ALL_DYN_LINK )
endif()