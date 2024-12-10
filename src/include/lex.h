#ifndef __LEX_H
#define __LEX_H

#include "string.h"

struct token {
    struct string lexeme;

    enum {
        TOK_EOF,        // End of File
        TOK_FUN,        // fun
        TOK_IDENT,      // identifier
        TOK_COLON,      // :
        TOK_INT,        // int
        TOK_RETURN,     // return
        TOK_NUM,        // number
        TOK_SEMICOLON,  // ;
        TOK_LBRACE,     // {
        TOK_RBRACE,     // }
        TOK_TILDA,      // ~
        TOK_DASH,       // -
        TOK_BANG,       // !
    } type;
};

void lex_init(char* ptr);
struct error next_token(struct token* tok);

#endif  //__LEX_H
