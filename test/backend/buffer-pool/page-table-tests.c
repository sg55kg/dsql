//
// Created by sdollar on 5/22/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "../../../backend/buffer-pool/page-table.h"

void insert_test() {
    PageTable* table = init_page_table();
    PageTableEntry* page = create_page_table_entry(1);
    insert_page(table, 1, page);
    const PageTableEntry* lookup_page = lookup_key(table, 1);
    assert(lookup_page != NULL);
    assert(lookup_page->page_number == 1);
    free(page);
    free(table);
    printf("Page table insert test succeeded\n");
}

void lookup_test() {
    PageTable* table = init_page_table();
    // setup
    for (int i = 0; i < HASH_TABLE_SIZE * 2; i++) {
        PageTableEntry* page = create_page_table_entry(i);
        insert_page(table, i, page);
    }

    // assertions
    assert(table->clock_hand != NULL);
    assert(table->size == HASH_TABLE_SIZE * 2);

    for (int i = 0; i < HASH_TABLE_SIZE * 2; i++) {
        const PageTableEntry* lookup = lookup_key(table, i);
        assert(lookup != NULL);
        assert(lookup->page_number == i);
        assert(lookup->reference_bit == 1);
    }

    // clean up
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        PageTableEntry* to_remove = table->table[i];
        if (to_remove == NULL) {
            continue;
        }
        PageTableEntry* next = to_remove->next;
        free(to_remove);
        while (next != NULL) {
            PageTableEntry* temp = next->next;
            free(next);
            next = temp;
        }
    }
    free(table);
    printf("All page table lookup tests succeeded\n");
}

int main() {
    insert_test();
    lookup_test();
    printf("All tests passed\n");
    return 0;
}