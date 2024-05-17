//
// Created by Sam on 5/17/24.
//
#include "token.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Token* token_create(const enum TokenType _type, const char* value) {
    struct Token* t = malloc(sizeof(struct Token));
    if (t == NULL) {
        perror("Error allocating memory for new token\n");
        return NULL;
    }
    t->next = NULL;
    t->prev = NULL;
    t->type = _type;
    t->value = malloc(strlen(value) + 1);
    if (t->value == NULL) {
        perror("Error allocating memory for token value\n");
        free(t);
        return NULL;
    }
    strcpy(t->value, value);
    return t;
}

void token_destroy(struct Token* token) {
    if (token == NULL) {
        return;
    }
    while (token->prev != NULL) {
        token = token->prev;
    }
    while (token->next != NULL) {
        free(token->value);
        struct Token* temp = token->next;
        free(token);
        token = temp;
    }
    free(token->value);
    free(token);
}
