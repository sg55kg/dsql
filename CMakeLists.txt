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
        backend/buffer-pool/buffer-pool.h
        backend/buffer-pool/page-table.c
        backend/buffer-pool/page-table.h
)

# Test executable source files
add_executable(FileManagerTests
        test/backend/file-manager/file-manager-tests.c
        backend/file-manager/file_manager.c)
add_executable(ParserTests test/backend/parser/parser-tests.c)

find_package(xxHash CONFIG REQUIRED)
target_link_libraries(dollar_sql PRIVATE xxHash::xxhash)
