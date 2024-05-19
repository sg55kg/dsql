//
// Created by Sam on 5/17/24.
//
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "token.h"

int main(const int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path_to_sql_file>\n", argv[0]);
        return 1;
    }

    const char *file_path = argv[1];
    FILE *file = fopen(file_path, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    printf("Opened the file\n");

    fseek(file, 0L, SEEK_END);
    const long file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    struct Token* tokens = parse_to_tokens(file, file_size);

    while (tokens->prev != NULL) {
        tokens = tokens->prev;
    }

    while (tokens->next != NULL) {
        printf(tokens->value);
        printf("\n");
        tokens = tokens->next;
    }
    printf(tokens->value);
    printf("\n");


    destroytokens:
        token_destroy(tokens);

    fileclose:
        fclose(file);

    return 0;
}