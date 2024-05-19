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

// TODO handle comments later

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

static struct Token* parse_state_none(struct Token* tokens, const char ch, char* curr_val, enum ParserState* state) {
    if (strcmp(curr_val, ";") == 0) { // TODO handle cases like " ;" trim string
        tokens = incr_tokens(tokens, SEMI, curr_val);
    } else if (ch == ' ') {
        if (strcmp(curr_val, "USING") == 0) {
            tokens = incr_tokens(tokens, USING, curr_val);
            *state = PS_USING;
        } else if (strcmp(curr_val, "SELECT") == 0) {
            tokens = incr_tokens(tokens, SELECT_COMMAND, curr_val);
            *state = PS_SUDI;
        } else if (strcmp(curr_val, "UPDATE") == 0) {
            tokens = incr_tokens(tokens, UPDATE_COMMAND, curr_val);
            *state = PS_SUDI;
        } else if (strcmp(curr_val, "DELETE") == 0) {
            tokens = incr_tokens(tokens, DELETE_COMMAND, curr_val);
            *state = PS_SUDI;
        } else if (strcmp(curr_val, "INSERTINTO") == 0) {
            tokens = incr_tokens(tokens, INSERT_COMMAND, curr_val);
            *state = PS_SUDI;
        } else if (strcmp(curr_val, "CREATE") == 0) {
            tokens = incr_tokens(tokens, CREATE_COMMAND, curr_val);
            *state = PS_CREATE;
        }
    } else if (strcmp(curr_val, "\n") == 0) {
        tokens = incr_tokens(tokens, NEW_LINE, curr_val);
    }

    return tokens;
}

static struct Token* parse_state_using(struct Token* tokens, const char ch, char* curr_val, enum ParserState* state) {
    if (ch == ';' || ch == ' ' || ch == '\n') {
        tokens = incr_tokens(tokens, SCHEMA_NAME, curr_val);
        *state = PS_NONE;
    } else if (ch == '.') {
        tokens = incr_tokens(tokens, DATABASE_NAME, curr_val);
    }

    return tokens;
}

static struct Token* parse_state_sudi(struct Token* tokens, const char ch, char* curr_val, enum ParserState* state) {
    if (ch == ';') {
        *state = PS_NONE;
    } else if (tokens->type == SELECT_COMMAND) {
        if (ch == ' ' && strlen(curr_val) > 0) {
            tokens = incr_tokens(tokens, COLUMNS, curr_val);
        }
    } else if (tokens->type == INSERT_COMMAND) {
        if (ch == ' ' && strlen(curr_val) > 0) {
            tokens = incr_tokens(tokens, COLUMNS, curr_val);
        }
    } else if (tokens->type == COLUMNS) {
        if (ch == ' ' && strcmp(curr_val, "FROM") == 0) {
            tokens = incr_tokens(tokens, FROM, curr_val);
        }
    } else if (tokens->type == FROM) {
        if (ch == ' ') {
            tokens = incr_tokens(tokens, TABLE_NAME, curr_val);
        }
    } else if (tokens->type == TABLE_NAME || tokens->type == TABLE_ALIAS) {
        if (ch == ' ') {
            if (strcmp(curr_val, "WHERE") == 0) {
                tokens = incr_tokens(tokens, FILTER_COMMAND, curr_val);
                *state = PS_FILTER;
            } else if (strcmp(curr_val, "ORDERBY") == 0) {
                tokens = incr_tokens(tokens, ORDER_COMMAND, curr_val);
                *state = PS_ORDER;
            } else if (strcmp(curr_val, "ORDER") == 0) {
                // ignore
            } else if (strcmp(curr_val, "GROUPBY") == 0) {
                tokens = incr_tokens(tokens, GROUP_COMMAND, curr_val);
                *state = PS_GROUP;
            } else if (strcmp(curr_val, "GROUP") == 0) {
                // ignore
            } else if (tokens->type != TABLE_ALIAS) {
                tokens = incr_tokens(tokens, TABLE_ALIAS, curr_val);
            } else if (strcmp(curr_val, "LEFT") == 0) {
                tokens = incr_tokens(tokens, JOIN_LEFT, curr_val);
                *state = PS_JOIN;
            } else if (strcmp(curr_val, "RIGHT") == 0) {
                tokens = incr_tokens(tokens, JOIN_RIGHT, curr_val);
                *state = PS_JOIN;
            } else if (strcmp(curr_val, "JOIN") == 0) {
                tokens = incr_tokens(tokens, JOIN, curr_val);
                *state = PS_JOIN;
            } else {
                *state = PS_NONE; // probably shouldn't reach this?
            }
        }
    }

    return tokens;
}

static struct Token* parse_state_create(struct Token* tokens, const char ch, char* curr_val, enum ParserState* state) {
    if (ch == ';') {
        *state = PS_NONE;
        if (tokens->type == COLUMN_NAME && curr_val[strlen(curr_val)-1] == ')') {
            tokens = incr_tokens(tokens, CREATE_DEFINITION, curr_val);
        }
    } else if (ch == ' ') {
        if (tokens->type == CREATE_COMMAND) {
            if (strcmp(curr_val, "DATABASE") == 0) {
                tokens = incr_tokens(tokens, CREATE_TYPE_DATABASE, curr_val);
            } else if (strcmp(curr_val, "SCHEMA") == 0) {
                tokens = incr_tokens(tokens, CREATE_TYPE_SCHEMA, curr_val);
            } else if (strcmp(curr_val, "TABLE") == 0) {
                tokens = incr_tokens(tokens, CREATE_TYPE_TABLE, curr_val);
            }
        } else if (tokens->type == CREATE_TYPE_DATABASE) {
            tokens = incr_tokens(tokens, DATABASE_NAME, curr_val);
        } else if (tokens->type == CREATE_TYPE_SCHEMA) {
            tokens = incr_tokens(tokens, SCHEMA_NAME, curr_val);
        } else if (tokens->type == CREATE_TYPE_TABLE) {
            tokens = incr_tokens(tokens, TABLE_NAME, curr_val);
        } else if (tokens->type == CREATE_DEFINITION) {
            tokens = incr_tokens(tokens, COLUMN_DATA_TYPE, curr_val);
        } else if (tokens->type == COLUMN_DATA_TYPE) {
            tokens = incr_tokens(tokens, COLUMN_NAME, curr_val);
        } else if (tokens->type == COLUMN_NAME && strcmp(curr_val, ",") == 0) {
            tokens = incr_tokens(tokens, COLUMN_SEPERATOR, curr_val);
        } else if (tokens->type == COLUMN_NAME && curr_val[strlen(curr_val)-1] == ')') {
            tokens = incr_tokens(tokens, CREATE_DEFINITION, curr_val);
        } else if (tokens->type == COLUMN_SEPERATOR) {
            tokens = incr_tokens(tokens, COLUMN_DATA_TYPE, curr_val);
        }
    } else if (ch == ',' && tokens->type == COLUMN_DATA_TYPE) {
        tokens = incr_tokens(tokens, COLUMN_NAME, curr_val);
    } else if (tokens->type == TABLE_NAME && strcmp(curr_val, "(") == 0) {
        tokens = incr_tokens(tokens, CREATE_DEFINITION, curr_val);
    } else if (tokens->type == COLUMN_DATA_TYPE && ch == ')') {
        tokens = incr_tokens(tokens, COLUMN_NAME, curr_val);
    }

    return tokens;
}

static struct Token* parse_state_filter(struct Token* tokens, const char ch, char* curr_val, enum ParserState* state) {
    if (ch == ';') {
        *state = PS_NONE;
    }

    return tokens;
}

static struct Token* parse_state_order(struct Token* tokens, const char ch, char* curr_val, enum ParserState* state) {
    if (ch == ';') {
        *state = PS_NONE;
    }

    return tokens;
}

static struct Token* parse_state_group(struct Token * tokens, const char ch, char * curr_val, enum ParserState* state) {
    if (ch == ';') {
        *state = PS_NONE;
    }

    return tokens;
}

static struct Token* parse_state_join(struct Token* tokens, const char ch, char* curr_val, enum ParserState* state) {
    if (ch == ';') {
        *state = PS_NONE;
    }

    return tokens;
}

struct Token* parse_to_tokens(FILE *file, const size_t file_size) {
    enum ParserState curr_state = PS_NONE;

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
        switch (curr_state) {
            case PS_NONE:
                tokens = parse_state_none(tokens, ch, curr_val, &curr_state);
                break;
            case PS_USING:
                tokens = parse_state_using(tokens, ch, curr_val, &curr_state);
                break;
            case PS_SUDI:
                tokens = parse_state_sudi(tokens, ch, curr_val, &curr_state);
                break;
            case PS_CREATE:
                tokens = parse_state_create(tokens, ch, curr_val, &curr_state);
                break;
            case PS_FILTER:
                tokens = parse_state_filter(tokens, ch, curr_val, &curr_state);
                break;
            case PS_ORDER:
                tokens = parse_state_order(tokens, ch, curr_val, &curr_state);
                break;
            case PS_GROUP:
                tokens = parse_state_group(tokens, ch, curr_val, &curr_state);
                break;
            case PS_JOIN:
                tokens = parse_state_join(tokens, ch, curr_val, &curr_state);
                break;
        }

        if (tokens == NULL) {
            free(curr_val);
            return NULL;
        }

        if (ch != ' ' && ch != '.') {
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
