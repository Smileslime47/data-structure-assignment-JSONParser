cmake_minimum_required(VERSION 3.20)
project(JSONLIB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")

add_executable(JSONLIB main.cpp Lexer.h Stream.h Token.h Map.h Parser.h AST.h)
