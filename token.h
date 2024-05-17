//
// Created by Sam on 5/17/24.
//

#ifndef TOKEN_H
#define TOKEN_H

enum TokenType {
    SEMI, // ;
    SELECT_COMMAND, // SELECT
    UPDATE_COMMAND, // UPDATE
    DELETE_COMMAND, // DELETE
    INSERT_COMMAND, // INSERT INTO
    FROM, // FROM
    TABLE_NAME, // any string
    TABLE_ALIAS, // any string
    FILTER_COMMAND, // WHERE
    FILTER_CONDITION, // col = val
    FILTER_AND, // AND
    FILTER_OR, // OR
    ORDER_COMMAND, // ORDER BY
    ORDER_VALUES, // a DESC
    COLUMNS, // * or (colA, colB)
    NONE
};

struct Token {
    enum TokenType type;
    char* value;
    struct Token* prev;
    struct Token* next;
};

struct Token* token_create(const enum TokenType _type, const char* value);
void token_destroy(struct Token* token);

#endif //TOKEN_H
