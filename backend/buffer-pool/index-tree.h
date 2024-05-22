//
// Created by sdollar on 5/22/2024.
//

#ifndef INDEX_TREE_H
#define INDEX_TREE_H

#include <stdint.h>

#define MAX_KEYS 5
#define MAX_CHILDREN (MAX_KEYS + 1)

typedef struct IndexTreeNode {
    struct IndexTreeNode* parent;
    uint64_t page_keys[MAX_KEYS];
    struct IndexTreeNode* children[MAX_CHILDREN];
    uint8_t is_leaf;
    uint16_t num_keys;
    struct IndexTreeNode* next_leaf;
} IndexTreeNode;

IndexTreeNode* find_page_keys(uint64_t index_key);
IndexTreeNode* create_itn();

#endif //INDEX_TREE_H
