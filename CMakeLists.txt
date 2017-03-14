cmake_minimum_required(VERSION 3.6)
project(DateTime)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp DateTime.h TimeSpan.h)
add_executable(DateTime ${SOURCE_FILES})