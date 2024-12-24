#include <ctype.h>
#include <stdio.h>

#include "include/lex.h"
#include "include/error.h"
#include "include/loc.h"
#include "include/string.h"

static char *g_start = 0, *g_current = 0;

struct location loc = {1, 0};

void lex_init(char* ptr) {
    g_start = ptr;
    g_current = g_start;
}

struct error next_token(struct token* tok) {
next_token:
    if (!tok)
        return ERROR(ERROR_NULLPTR, "NULL tok was passed into next_token", loc);

    if (g_start == 0 || g_current == 0)
        return ERROR(ERROR_GENERIC, "Lexer not initialized", loc);

    g_start = g_current;

    if (*g_current == '\0') {
        tok->type = TOK_EOF; goto make_tok;
    }

    // Skipping whitespace
    char c = *g_current++;
    loc.column++;

    // ' ' '\t' '\n'
    if (isspace(c)) {
        switch (c) {
            case '\t': loc.column+=7; break;
            case '\n': loc.line++; loc.column = 0; break;
            default: break;
        }

        goto next_token;
    }

    switch (c) {
        case ':': tok->type = TOK_COLON; goto make_tok;
        case ';': tok->type = TOK_SEMICOLON; goto make_tok;
        case '{': tok->type = TOK_LBRACE; goto make_tok;
        case '}': tok->type = TOK_RBRACE; goto make_tok;

        case '~': tok->type = TOK_TILDA; goto make_tok;
        case '-': tok->type = TOK_DASH; goto make_tok;
        case '!': tok->type = TOK_BANG; goto make_tok;
    }

    if (isalpha(c)) {
        char* ptr = g_current; 
        while (isalnum(*ptr)) {
            ptr = ++g_current;
            loc.column++;
        }
        tok->type = TOK_IDENT;
        tok->lexeme = STRING(g_start, g_current-g_start);
        tok->loc = loc;
        tok->loc.column -= tok->lexeme.len-1;

        // Check the keywords
        // fun int return
        if (string_eq(tok->lexeme, string("fun"))) {
            tok->type = TOK_FUN;
        } else if (string_eq(tok->lexeme, string("int"))) {
            tok->type = TOK_INT;
        } else if (string_eq(tok->lexeme, string("return"))) {
            tok->type = TOK_RETURN;
        }

        goto ok;
    }

    if (isdigit(c)) {
        char* ptr = g_current; 
        while (isdigit(*ptr)) {
            ptr = ++g_current;
            loc.column++;
        }
        tok->type = TOK_NUM;
        goto make_tok;
    }

    return ERROR(ERROR_GENERIC, "Unknown character", loc);

make_tok:
    tok->lexeme = STRING(g_start, g_current-g_start);
    tok->loc = loc;
ok:
    printf("%.*s", tok->lexeme.len, tok->lexeme.ptr);
    location_print(tok->loc);
    return ERROR_OK;
}
