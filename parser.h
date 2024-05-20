//
// Created by Sam on 5/17/24.
//

#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include "token.h"

/// Current type of SQL command the parser is handling
enum ParserState {
    PS_NONE,
    PS_USING,
    PS_CREATE,
    PS_SUD, // Select, update, delete TODO might break up
    PS_INSERT,
    PS_FILTER,
    PS_ORDER,
    PS_GROUP,
    PS_JOIN,
};

struct Token* incr_tokens(struct Token* tokens, enum TokenType type, char* val);
struct Token* parse_to_tokens(FILE *file, size_t file_size);

#endif //PARSER_H
