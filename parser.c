//
// Created by Sam on 5/17/24.
//
//
// Created by Sam on 5/17/24.
//
#include "parser.h"
#include <stdlib.h>
#include <string.h>

#include "token.h"

struct Token* incr_tokens(struct Token* tokens, const enum TokenType type, char* val) {
    struct Token* token = token_create(type, val);
    if (token == NULL) {
        token_destroy(tokens);
        return NULL;
    }
    token->prev = tokens;
    tokens->next = token;
    val[0] = '\0';
    return token;
}

struct Token* parse_to_tokens(FILE *file, const size_t file_size) {
    struct Token* start_token = token_create(NONE, "");
    if (start_token == NULL) {
        return NULL;
    }

    const size_t buffer_size = file_size + 1;
    char* curr_val = malloc(buffer_size);
    if (curr_val == NULL) {
        perror("Memory allocation for current value failed");
        free(start_token);
        return NULL;
    }
    curr_val[0] = '\0';
    struct Token* tokens = start_token;

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
        switch (tokens->type) {
            case SELECT_COMMAND:
                if (ch == ' ' && strcmp(curr_val, "") != 0) {
                    tokens = incr_tokens(tokens, COLUMNS, curr_val);
                }
                break;
            case UPDATE_COMMAND:
                if (ch == '*') {
                    tokens = incr_tokens(tokens, COLUMNS, curr_val);
                } else if (ch == ')') {
                    tokens = incr_tokens(tokens, COLUMNS, curr_val);
                }
                break;
            case DELETE_COMMAND:
                if (strlen(curr_val) > 0 && ch == ' ') {
                    tokens = incr_tokens(tokens, TABLE_NAME, curr_val);
                }
                break;
            case INSERT_COMMAND:
                if (strlen(curr_val) > 0 && ch == ' ') {
                    tokens = incr_tokens(tokens, TABLE_NAME, curr_val);
                }
                break;
            case SEMI:
                break;
            case TABLE_NAME:
                if (ch == ';' && strcmp(curr_val, "") == 0) {
                    tokens = incr_tokens(tokens, SEMI, curr_val);
                } else if (ch == ';' && strcmp(curr_val, "") != 0) {
                    tokens = incr_tokens(tokens, TABLE_ALIAS, curr_val);
                } else if (ch == ' ' && strcmp(curr_val, "") != 0) {
                    tokens = incr_tokens(tokens, TABLE_ALIAS, curr_val);
                }
                break;
            case TABLE_ALIAS:
                if (ch == ';') {
                    tokens = incr_tokens(tokens, SEMI, ";");
                }
                break;
            case FILTER_COMMAND:
                if (strcmp(curr_val, "AND") == 0) {
                    tokens = incr_tokens(tokens, FILTER_AND, curr_val);
                } else if (strcmp(curr_val, "OR") == 0) {
                    tokens = incr_tokens(tokens, FILTER_OR, curr_val);
                }
                break;
            case FILTER_CONDITION:
                if (ch == ';') {
                    tokens = incr_tokens(tokens, SEMI, curr_val);
                }
                break;
            case FILTER_AND:
                break;
            case FILTER_OR:
                break;
            case ORDER_COMMAND:
                break;
            case ORDER_VALUES:
                if (ch == ';') {

                }
                break;
            case COLUMNS:
                if (ch == ' ' && strcmp(curr_val, "FROM") == 0) {
                    tokens = incr_tokens(tokens, FROM, curr_val);
                }
                break;
            case FROM:
                if (ch == ' ' && strcmp(curr_val, "") != 0) {
                    tokens = incr_tokens(tokens, TABLE_NAME, curr_val);
                }
                break;
            case NONE:
                if (strcmp(curr_val, "SELECT") == 0) {
                    tokens = incr_tokens(tokens, SELECT_COMMAND, curr_val);
                } else if (strcmp(curr_val, "UPDATE") == 0) {
                    tokens = incr_tokens(tokens, UPDATE_COMMAND, curr_val);
                } else if (strcmp(curr_val, "DELETE") == 0) {
                    tokens = incr_tokens(tokens, DELETE_COMMAND, curr_val);
                } else if (strcmp(curr_val, "INSERT INTO") == 0) {
                    tokens = incr_tokens(tokens, INSERT_COMMAND, curr_val);
                }
                break;
        }
        if (tokens == NULL) {
            free(curr_val);
            return NULL;
        }

        if (ch != ' ') {
            const size_t len = strlen(curr_val);
            curr_val[len] = ch;
            curr_val[len+1] = '\0';
        }
    }

    if (strcmp(curr_val, ";") == 0) {
        tokens = incr_tokens(tokens, SEMI, curr_val);
    }

    free(curr_val);

    return tokens;
}
