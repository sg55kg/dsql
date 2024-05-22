//
// Created by sdollar on 5/22/2024.
//

#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "page.h"

Page* read_page(uint64_t key);
void write_page(Page* page);

#endif //PAGE_MANAGER_H
