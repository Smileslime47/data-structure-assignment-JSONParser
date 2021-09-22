cmake_minimum_required(VERSION 3.20)
project(JSONLIB)

set(CMAKE_CXX_STANDARD 20)

add_executable(JSONLIB main.cpp Lexer.h Stream.h Token.h Map.h Parser.h AST.h)
