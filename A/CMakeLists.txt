# cmake_minimum_required(VERSION <specify CMake version here>)
project(A)

set(CMAKE_CXX_STANDARD 17)

add_executable(A main.cpp Util.cpp Util.h LambdaParser.cpp LambdaParser.h main.h)