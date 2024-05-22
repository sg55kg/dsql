//
// Created by sdollar on 5/21/2024.
//

#include "page-table.h"

#include <stdio.h>
#include <stdlib.h>
#include <xxhash.h>

#define KEY_FOUND 1
#define KEY_NOT_FOUND 0

// TODO - implement LRU list

static XXH64_hash_t get_hash(const uint64_t key) {
    const XXH64_hash_t hash = XXH64(&key, sizeof(key), 0);
    return hash % HASH_TABLE_SIZE;
}

PageTableEntry* create_page_table_entry(const uint64_t key) {
    PageTableEntry* entry = malloc(sizeof(PageTableEntry));
    if (entry == NULL) {
        fprintf(stderr, "Error allocating memory for page table entry");
        return NULL;
    }
    entry->page_number = key;
    entry->clock_next = NULL;
    entry->next = NULL;
    entry->prev = NULL;
    entry->reference_bit = 0;
    entry->pin_count = 0;
    entry->is_dirty = 0;
    return entry;
}

PageTable* init_page_table() {
    PageTable* table = malloc(sizeof(PageTable));
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        table->table[i] = NULL;
    }
    table->clock_hand = NULL;
    table->size = 0;
    return table;
}

int has_key(const PageTable* table, const uint64_t key) {
    const XXH64_hash_t hash = get_hash(key);
    PageTableEntry* entry = table->table[hash];

    while (entry != NULL) {
        if (entry->page_number == key) {
            entry->reference_bit = 1;
            return KEY_FOUND;
        }
        entry = entry->next;
    }

    return KEY_NOT_FOUND;
}

PageTableEntry* lookup_key(const PageTable* table, const uint64_t key) {
    const XXH64_hash_t hash = get_hash(key);
    PageTableEntry* entry = table->table[hash];

    while (entry != NULL) {
        if (entry->page_number == key) {
            entry->reference_bit = 1;
            return entry;
        }
        entry = entry->next;
    }

    return NULL;
}

void insert_page(PageTable* table, const uint64_t key, PageTableEntry* page) {
    const XXH64_hash_t hash = get_hash(key);
    PageTableEntry* existing_entry = table->table[hash];
    page->reference_bit = 1;
    page->next = existing_entry;
    table->table[hash] = page;

    if (table->clock_hand != NULL) {
        page->clock_next = table->clock_hand->clock_next;
        table->clock_hand->clock_next = page;
    } else {
        page->clock_next = page;
        table->clock_hand = page;
    }

    table->size++;
}
