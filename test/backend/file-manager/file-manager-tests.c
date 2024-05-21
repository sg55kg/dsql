//
// Created by sdollar on 5/21/2024.
//
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <assert.h>
#else
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#endif

#include "../../../backend/file-manager/file_manager.h"

#define TEST_PAGE_SIZE 4096 // 4kb

#if _WIN32
void* allocate_aligned_buffer(const size_t size, const size_t alignment) {
    return _aligned_malloc(size, alignment);
}

void free_aligned_buffer(void* buffer) {
    _aligned_free(buffer);
}

#else

void* allocate_aligned_buffer(size_t size, size_t alignment) {
    void* buffer = NULL;
    if (posix_memalign(&buffer, alignment, size) != 0) {
        perror("Failed to allocate aligned memory");
        return NULL;
    }
    return buffer;
}

void free_aligned_buffer(void* buffer) {
    free(buffer);
}

#endif

void test_create_file() {
    const char* filename = "testfile.dat";
    const int fd = create_file(filename);
    assert(fd != -1);
    printf("File creation test passed.\n");
    close_file(fd);
}

void test_open_file() {
    const char* filename = "testfile.dat";
    const int fd = open_file(filename);
    assert(fd != -1);
    printf("File opening test passed.\n");
    close_file(fd);
}

void test_write_file() {
    const char* filename = "testfile.dat";
    const int fd = open_file(filename);
    assert(fd != -1);

    const size_t buffer_size = TEST_PAGE_SIZE;
    void* buffer = allocate_aligned_buffer(buffer_size, SECTOR_SIZE);
    assert(buffer != NULL);
    memset(buffer, 'A', buffer_size);

    const int result = write_file(fd, buffer, buffer_size);
    assert(result == 0);
    printf("File writing test passed.\n");

    free_aligned_buffer(buffer);
    close_file(fd);
}

void test_read_file() {
    const char* filename = "testfile.dat";
    const int fd = open_file(filename);
    assert(fd != -1);

    const size_t buffer_size = TEST_PAGE_SIZE;
    void* buffer = allocate_aligned_buffer(buffer_size, SECTOR_SIZE);
    assert(buffer != NULL);
    memset(buffer, 0, buffer_size);

    const int result = read_file(fd, buffer, buffer_size);
    assert(result == 0);
    assert(((char*)buffer)[0] == 'A');
    printf("File reading test passed.\n");

    free_aligned_buffer(buffer);
    close_file(fd);
}

void test_close_file() {
    const char* filename = "testfile.dat";
    const int fd = open_file(filename);
    assert(fd != -1);

    const int result = close_file(fd);
    assert(result == 0);
    printf("File closing test passed.\n");
}

int main() {
    test_create_file();
    test_open_file();
    test_write_file();
    test_read_file();
    test_close_file();

    printf("All tests passed.\n");
    return 0;
}