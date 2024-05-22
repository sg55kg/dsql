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
    uint8_t reference_bit; // for clock
    uint32_t pin_count;
    struct PageTableEntry* clock_next;
    struct PageTableEntry* prev;
    struct PageTableEntry* next;
} PageTableEntry;

typedef struct {
    PageTableEntry* table[HASH_TABLE_SIZE]; // TODO replace with non-static size
    PageTableEntry* clock_hand;
    uint64_t size;
} PageTable;

PageTableEntry* create_page_table_entry(const uint64_t key);
int has_key(const PageTable* table, const uint64_t key);
PageTableEntry* lookup_key(const PageTable* table, const uint64_t key);
void insert_page(PageTable* table, const uint64_t key, PageTableEntry* page);
PageTable* init_page_table();

#endif //PAGE_TABLE_H
