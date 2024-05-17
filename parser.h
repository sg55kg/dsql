//
// Created by Sam on 5/17/24.
//

#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include "token.h"

struct Token* incr_tokens(struct Token* tokens, enum TokenType type, char* val);
struct Token* parse_to_tokens(FILE *file, size_t file_size);

#endif //PARSER_H
