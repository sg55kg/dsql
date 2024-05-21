//
// Created by sdollar on 5/21/2024.
//

#ifndef PAGE_H
#define PAGE_H

#define PAGE_SIZE 4096 // 4 KB
#include "stdint.h"

typedef struct PageDataPointer {
    unsigned tuple_offset:15;
    unsigned flags:2;
    unsigned tuple_len:15;
} PageDataPointer;

typedef struct PageHeader {
    int16_t page_version;
    int16_t flags;
    int16_t data_lower;
    int16_t data_upper;
    PageDataPointer line_pointers[];
} PageHeader;

typedef struct Page {
    char data[PAGE_SIZE]; // TODO - does this need to be char?
} Page;

// page flags
#define PAGE_HAS_FREE_LINES	0x0001
#define PAGE_FULL		    0x0002

#endif //PAGE_H
