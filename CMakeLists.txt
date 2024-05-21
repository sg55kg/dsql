cmake_minimum_required(VERSION 3.28)
project(dollar_sql C)

set(CMAKE_C_STANDARD 11)

add_executable(dollar_sql main.c
        backend/parser/parser.h
        backend/parser/token.h
        backend/parser/token.c
        backend/parser/parser.c
        backend/file-manager/file_manager.h
        backend/file-manager/file_manager.c
        backend/page-manager/page.h
)
