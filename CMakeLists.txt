cmake_minimum_required(VERSION 3.28)
project(dollar_sql C)

set(CMAKE_C_STANDARD 11)

add_executable(dollar_sql main.c
        backend/parser/parser.h
        backend/parser/token.h
        backend/parser/token.c
        backend/parser/parser.c
        backend/storage/file_manager.h
        backend/storage/file_manager.c
        backend/storage/data-page.h
        backend/buffer-pool/buffer-pool.h
        backend/buffer-pool/page-table.c
        backend/buffer-pool/page-table.h
        backend/storage/page-manager.h
        backend/storage/page-manager.c
        backend/buffer-pool/index-tree.c
        backend/buffer-pool/index-tree.h
        backend/storage/index-page.c
        backend/storage/index-page.h
        backend/storage/util.c
        backend/storage/util.h
)

# Test executable source files
add_executable(FileManagerTests
        test/backend/storage/file-manager-tests.c
        backend/storage/file_manager.c)
add_executable(ParserTests test/backend/parser/parser-tests.c)
add_executable(PageTableTests
        test/backend/buffer-pool/page-table-tests.c
        backend/buffer-pool/page-table.c)

find_package(xxHash CONFIG REQUIRED)
target_link_libraries(dollar_sql PRIVATE xxHash::xxhash)
target_link_libraries(PageTableTests PRIVATE xxHash::xxhash)
add_executable(PageManagerTests test/backend/storage/page-manager-tests.c)
