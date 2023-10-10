cmake_minimum_required(VERSION 3.21)

project(Machine)

set(CMAKE_CXX_STANDARD 20)

set(MACHINE_SOURCES machine.cpp)

add_library(machine SHARED ${MACHINE_SOURCES})
