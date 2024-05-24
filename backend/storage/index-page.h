//
// Created by sdollar on 5/24/2024.
//

#ifndef INDEX_PAGE_H
#define INDEX_PAGE_H

#include <stdint.h>
#define INDEX_PAGE_SIZE 4096 // 4KB

typedef struct {
    uint64_t key;
    uint16_t child_offset;
} IndexEntry;

typedef struct IndexPageHeader {
    uint16_t version;
    uint16_t flags;
    uint16_t num_entries;
    uint8_t is_leaf;
    int16_t free_space_offset;
    IndexEntry entries[];
} IndexPageHeader;

typedef struct IndexPage {
    char data[INDEX_PAGE_SIZE];
} IndexPage;

#endif //INDEX_PAGE_H
