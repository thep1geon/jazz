#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "include/lex.h"
#include "include/error.h"
#include "include/ast.h"


struct ast* parse_number(struct token tok);
struct ast* parse_unary_op(struct token tok);
struct ast* parse_expr();
struct ast* parse_return_stmt();
struct ast* parse_stmt(struct token tok);
struct ast* parse_fun_decl();
struct ast* parse_decl(struct token tok);
struct ast* parse_program();
struct ast* parse(char* src);

struct ast* parse_number(struct token tok) {
    struct string str = tok.lexeme;
    char c = str.ptr[str.len];
    str.ptr[str.len] = '\0';

    int n = atoi(str.ptr);
    str.ptr[str.len] = c;

    return ast_number_alloc(n);
}

struct ast* parse_unary_op(struct token tok) {
    return ast_unary_op_alloc(tok, parse_expr());
}

struct ast* parse_expr() {
    struct token tok;
    struct error error = next_token(&tok);
    if (error.type != ERROR_NONE) {
        set_gerror(error);
        return NULL;
    }

    switch (tok.type) {
        case TOK_TILDA:
        case TOK_BANG:
        case TOK_DASH: return parse_unary_op(tok);

        case TOK_NUM: return parse_number(tok);
        default: assert(0 && "Unreachable");
    }

    assert(0 && "Unreachable");
    return NULL;
}

struct ast* parse_return_stmt() {
    struct ast* expr = parse_expr();
    if (!expr) return NULL;

    return ast_return_stmt_alloc(expr);
}

struct ast* parse_stmt(struct token tok) {
    struct ast* ast;
    switch (tok.type) {
        case TOK_RETURN: ast = parse_return_stmt(); break;
        default: assert(0 && "Unreachable");
    }

    struct error error = next_token(&tok);
    if (error.type != ERROR_NONE) {
        set_gerror(error);
        return NULL;
    }

    if (tok.type != TOK_SEMICOLON) {
        set_gerror(ERROR(ERROR_SYNTAX, "Expected semicolon", tok.loc));
        ast_free(ast);
        return NULL;
    }

    return ast;
}

struct ast* parse_fun_decl() {
    struct token tok;
    struct error error = next_token(&tok);
    if (error.type != ERROR_NONE) {
        set_gerror(error);
        return NULL;
    }

    if (tok.type != TOK_IDENT) {
        set_gerror(ERROR(ERROR_SYNTAX, "Expected identifier", tok.loc));
        return NULL;
    }

    struct string name = tok.lexeme;
    struct ast* ast = ast_fun_decl_alloc(name);

    error = next_token(&tok);
    if (error.type != ERROR_NONE) {
        set_gerror(error);
        return NULL;
    }
    if (tok.type != TOK_COLON) {
        set_gerror(ERROR(ERROR_SYNTAX, "Expected Colon", tok.loc));
        ast_free(ast);
        return NULL;
    }

    error = next_token(&tok);
    if (error.type != ERROR_NONE) {
        set_gerror(error);
        return NULL;
    }
    if (tok.type != TOK_INT) {
        set_gerror(ERROR(ERROR_SYNTAX, "Expected return type", tok.loc));
        ast_free(ast);
        return NULL;
    }

    error = next_token(&tok);
    if (error.type != ERROR_NONE) {
        set_gerror(error);
        return NULL;
    }
    if (tok.type != TOK_LBRACE) {
        set_gerror(ERROR(ERROR_SYNTAX, "Expected Left Brace", tok.loc));
        ast_free(ast);
        return NULL;
    }
    
    while (next_token(&tok).type == ERROR_NONE && tok.type != TOK_RBRACE) {
        if (tok.type == TOK_EOF) {
            set_gerror(ERROR(ERROR_SYNTAX, "Missing closing brace", tok.loc));
            ast_free(ast);
            return NULL;
        }

        struct ast* stmt = parse_stmt(tok);
        if (!stmt) {
            ast_free(ast);
            return NULL;
        }
        ast_da_append(&ast->as.fun_decl.stmts, stmt);
    }

    return ast;
}

struct ast* parse_decl(struct token tok) {
    switch (tok.type) {
        case TOK_FUN: return parse_fun_decl();
        default: assert(0 && "Unreachable");
    }
}

struct ast* parse_program() {
    struct ast* program = ast_progam_alloc();
    
    struct token tok;
    while (next_token(&tok).type == ERROR_NONE && tok.type != TOK_EOF) {
        struct ast* ast = parse_decl(tok);
        if (!ast) {
            ast_free(program);
            return NULL;
        }
        ast_da_append(&program->as.program.decls, ast);
    }

    return program;
}

struct ast* parse(char* src) {
    lex_init(src);
    return parse_program();
}
