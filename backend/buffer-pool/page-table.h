//
// Created by sdollar on 5/21/2024.
//

#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <stdint.h>

#define HASH_TABLE_SIZE 256 // TODO - generate this size by scanning index files

typedef struct PageTableEntry {
    uint64_t page_number;
    int frame_index;
    uint8_t is_dirty;
    uint32_t pin_count;
    struct PageTableEntry* prev;
    struct PageTableEntry* next;
} PageTableEntry;

typedef struct {
    PageTableEntry* table[HASH_TABLE_SIZE]; // TODO replace with non-static size
    PageTableEntry* lru_head; // TODO - replace LRU list with a clock system later
    PageTableEntry* lru_tail;
} PageTable;

int has_key(PageTable* table, const uint64_t key);
PageTableEntry* lookup_key(PageTable* table, uint64_t key);
void insert_page(PageTable* table, uint64_t key, PageTableEntry* page);
PageTable* init_page_table();

#endif //PAGE_TABLE_H
