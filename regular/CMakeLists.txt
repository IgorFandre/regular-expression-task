cmake_minimum_required(VERSION 3.21)

project(Regular)

set(CMAKE_CXX_STANDARD 20)

set(REGULAR_SOURCES regular.cpp)

add_library(regular SHARED ${REGULAR_SOURCES})