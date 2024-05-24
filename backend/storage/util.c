//
// Created by sdollar on 5/24/2024.
//

#include "util.h"

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
