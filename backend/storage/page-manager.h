//
// Created by sdollar on 5/22/2024.
//

#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "data-page.h"
#include "index-page.h"

DataPage* read_data_page(uint64_t key);
void write_data_page(DataPage* page);
IndexPage* read_index_page(uint64_t key);
void write_index_page(IndexPage* page);

#endif //PAGE_MANAGER_H
