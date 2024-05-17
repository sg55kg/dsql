cmake_minimum_required(VERSION 3.28)
project(dollar_sql C)

set(CMAKE_C_STANDARD 11)

add_executable(dollar_sql main.c
        parser.h
        token.h
        token.c
        parser.c)
