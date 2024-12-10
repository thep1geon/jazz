#include <ctype.h>

#include "include/lex.h"
#include "include/error.h"

static char *g_start = 0, *g_current = 0;

void lex_init(char* ptr) {
    g_start = ptr;
    g_current = g_start;
}

const char* whitespace = " \n\t\r\b";

struct error next_token(struct token* tok) {
next_token:
    if (!tok)
        return ERROR(ERROR_NULLPTR, "NULL tok was passed into next_token");

    if (g_start == 0 || g_current == 0)
        return ERROR(ERROR_GENERIC, "Lexer not initialized");

    g_start = g_current;

    if (*g_current == '\0') {
        tok->type = TOK_EOF;
        return ERROR_OK;
    }

    // Skipping whitespace
    char c = *g_current++;

    if (isspace(c)) goto next_token;

    switch (c) {
        case ':': {
            tok->lexeme = STRING(g_start, g_current - g_start);
            tok->type = TOK_COLON;
            return ERROR_OK;
        } break;

        case ';': {
            tok->lexeme = STRING(g_start, g_current - g_start);
            tok->type = TOK_SEMICOLON;
            return ERROR_OK;
        } break;

        case '{': {
            tok->lexeme = STRING(g_start, g_current - g_start);
            tok->type = TOK_LBRACE;
            return ERROR_OK;
        } break;

        case '}': {
            tok->lexeme = STRING(g_start, g_current - g_start);
            tok->type = TOK_RBRACE;
            return ERROR_OK;
        } break;
        case '~': {
            tok->lexeme = STRING(g_start, g_current - g_start);
            tok->type = TOK_TILDA;
            return ERROR_OK;
        } break;
        case '-': {
            tok->lexeme = STRING(g_start, g_current - g_start);
            tok->type = TOK_DASH;
            return ERROR_OK;
        } break;
        case '!': {
            tok->lexeme = STRING(g_start, g_current - g_start);
            tok->type = TOK_BANG;
            return ERROR_OK;
        } break;
    }

    if (isalpha(c)) {
        char* ptr = g_current; 
        while (isalnum(*ptr)) {
            ptr = ++g_current;
        }
        tok->lexeme = STRING(g_start, g_current - g_start);
        tok->type = TOK_IDENT;

        // Check the keywords
        // fun int return
        if (string_eq(tok->lexeme, string("fun"))) {
            tok->type = TOK_FUN;
        } else if (string_eq(tok->lexeme, string("int"))) {
            tok->type = TOK_INT;
        } else if (string_eq(tok->lexeme, string("return"))) {
            tok->type = TOK_RETURN;
        }

        return ERROR_OK;
    }

    if (isdigit(c)) {
        char* ptr = g_current; 
        while (isdigit(*ptr)) {
            ptr = ++g_current;
        }
        tok->lexeme = STRING(g_start, g_current - g_start);
        tok->type = TOK_NUM;
        return ERROR_OK;
    }

    return ERROR(ERROR_GENERIC, "Unknown character");
}
