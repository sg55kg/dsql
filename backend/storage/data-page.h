//
// Created by sdollar on 5/21/2024.
//
/*
 * This is for row/tuple storage
 */
#ifndef PAGE_H
#define PAGE_H

#include "stdint.h"

#define DATA_PAGE_SIZE 4096 // 4KB

typedef struct PageDataPointer {
    unsigned tuple_offset:15;
    unsigned flags:2;
    unsigned tuple_len:15;
} PageDataPointer;

typedef struct DataPageHeader {
    int16_t page_version;
    int16_t flags;
    int16_t data_lower;
    int16_t data_upper;
    PageDataPointer line_pointers[];
} DataPageHeader;

typedef struct DataPage {
    char data[DATA_PAGE_SIZE]; // TODO - does this need to be char?
} DataPage;

DataPage* read_page(uint64_t key);
void write_page(DataPage* page);

// page flags
#define PAGE_HAS_FREE_LINES	0x0001
#define PAGE_FULL		    0x0002

#endif //PAGE_H
