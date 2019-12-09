cmake_minimum_required(VERSION 3.15)
project(C)

set(CMAKE_CXX_STANDARD 17)

add_executable(C main.cpp LambdaParser.cpp LambdaParser.h main.h Equation.cpp Equation.h Type.cpp Type.cpp Type.h Substitution.cpp Substitution.h)