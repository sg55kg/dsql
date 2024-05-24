//
// Created by sdollar on 5/24/2024.
//

#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

void* allocate_aligned_buffer(const size_t size, const size_t alignment);
void free_aligned_buffer(void* buffer);
#endif //UTIL_H
