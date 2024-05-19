//
// Created by Sam on 5/17/24.
//

#ifndef TOKEN_H
#define TOKEN_H

enum TokenType {
    SEMI, // ;
    USING, // USING <database.schema>
    SELECT_COMMAND, // SELECT
    UPDATE_COMMAND, // UPDATE
    DELETE_COMMAND, // DELETE
    INSERT_COMMAND, // INSERT INTO
    CREATE_COMMAND, // CREATE
    CREATE_TYPE_TABLE, // (CREATE) TABLE
    CREATE_TYPE_DATABASE, // (CREATE) DATABASE
    CREATE_TYPE_SCHEMA, // (CREATE) SCHEMA
    CREATE_DEFINITION, //
    IN, // (CREATE TABLE) IN <schema>
    SCHEMA_NAME, // any string
    DATABASE_NAME, // any string
    FROM, // FROM
    TABLE_NAME, // any string
    TABLE_ALIAS, // any string
    COLUMN_DATA_TYPE, // INT (id), VARCHAR(20) (first_name)
    COLUMN_NAME, // (INT) id, (VARCHAR(20)) first_name
    COLUMN_SEPERATOR, // ,
    PRIMARY_KEY, // PRIMARY KEY
    PRIMARY_KEY_VALUE, // (PRIMARY KEY) (id)
    FOREIGN_KEY, // FOREIGN KEY
    FOREIGN_KEY_VALUE, // (FOREIGN KEY) (user_id)
    DEFAULT, // DEFAULT
    DEFAULT_VALUE, // (DEFAULT) ''
    COLUMN_NULLABLE, // (DEFAULT '') NOT NULL
    FILTER_COMMAND, // WHERE
    FILTER_CONDITION, // col = val, col > val, etc
    FILTER_AND, // AND
    FILTER_OR, // OR
    ORDER_COMMAND, // ORDER BY
    ORDER_VALUES, // (colA)
    ORDER_DIRECTION, // DESC
    COLUMNS, // * or (colA, colB) used by select, update, delete, insert
    GROUP_COMMAND, // GROUP BY
    GROUP_VALUES, // (colA, colB)
    JOIN_INNER,
    JOIN_OUTER,
    JOIN_LEFT,
    JOIN_RIGHT,
    JOIN,
    COMMENT, // --
    NEW_LINE, // \n
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
