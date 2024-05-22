//
// Created by sdollar on 5/21/2024.
//

#include "page-table.h"

#include <stdlib.h>
#include <xxhash.h>

#define KEY_FOUND 1
#define KEY_NOT_FOUND 0

// TODO - implement LRU list

static XXH64_hash_t get_hash(const uint64_t key) {
    const XXH64_hash_t hash = XXH64(&key, HASH_TABLE_SIZE, 0);
    return hash;
}

PageTable* init_page_table() {
    PageTable* table = malloc(sizeof(PageTable));
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        table->table[i] = NULL;
    }
    table->lru_head = NULL;
    table->lru_tail = NULL;
    return table;
}

int has_key(const PageTable* table, const uint64_t key) {
    const XXH64_hash_t hash = get_hash(key);
    const PageTableEntry* entry = table->table[hash];

    while (entry != NULL) {
        if (entry->page_number == key) {
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
            return entry;
        }
        entry = entry->next;
    }

    return NULL;
}

void insert_page(PageTable* table, const uint64_t key, PageTableEntry* page) {
    const XXH64_hash_t hash = get_hash(key);
    PageTableEntry* existing_entry = table->table[hash];

    if (existing_entry == NULL) {
        table->table[hash] = page;
        return;
    }

    while (existing_entry->next != NULL) {
        existing_entry = existing_entry->next;
    }

    page->prev = existing_entry;
    existing_entry->next = page;
}
