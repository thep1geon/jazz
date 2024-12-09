#ifndef __AST_H
#define __AST_H

#include <stdio.h>

#include "string.h"
#include "ast_da.h"

struct ast {
    enum {
        AST_PROGAM,
        AST_FUN_DECL,
        AST_RETURN_STMT,
        AST_NUMBER,
    } tag;

    union {
        struct ast_program { 
            struct ast_da decls; 
        } program; 

        struct ast_fun_decl { 
            struct string name; 
            struct ast_da stmts;
        } fun_decl;

        struct ast_return_stmt {
            struct ast* expr;
        } return_stmt;

        struct ast_number {
            int n; 
        } number;
    } as;
};

struct ast* ast_progam_alloc();
struct ast* ast_func_decl_alloc(struct string name);
struct ast* ast_return_stmt_alloc(struct ast* expr);
struct ast* ast_number_alloc(int n);

void ast_free(struct ast* ast);

void _pretty_print(struct ast* ast, int indent);
void pretty_print(struct ast* ast);

// Part of code generation
// Maybe this belongs in its own module :P
void femit(FILE* file, struct ast* ast);

#endif  //__AST_H
