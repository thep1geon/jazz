#include <stdlib.h>

#include "include/ast.h"
#include "include/ast_da.h"
#include "include/error.h"
#include "include/string.h"

struct ast* ast_progam_alloc() {
    struct ast* ast = malloc(sizeof(struct ast));
    ast->tag = AST_PROGAM;
    ast->as.program = (struct ast_program){ast_da_alloc(4)};
    return ast;
}

struct ast* ast_func_decl_alloc(struct string name) {
    struct ast* ast = malloc(sizeof(struct ast));
    ast->tag = AST_FUN_DECL;
    ast->as.fun_decl = (struct ast_fun_decl){name, ast_da_alloc(4)};
    return ast;
}

struct ast* ast_return_stmt_alloc(struct ast* expr) {
    struct ast* ast = malloc(sizeof(struct ast));
    ast->tag = AST_RETURN_STMT;
    ast->as.return_stmt = (struct ast_return_stmt){expr};
    return ast;
}

struct ast* ast_number_alloc(int n) {
    struct ast* ast = malloc(sizeof(struct ast));
    ast->tag = AST_NUMBER;
    ast->as.number = (struct ast_number){n};
    return ast;
}

void ast_free(struct ast* ast) {
    if (!ast) {
        set_gerror(ERROR(ERROR_NULLPTR, "NULL ast passed to ast_free"));
    }

    switch (ast->tag) {
        case AST_PROGAM: {
            struct ast_program program = ast->as.program;
            ast_da_free(&program.decls);
        }; break;
        case AST_FUN_DECL: {
            struct ast_fun_decl fun_decl = ast->as.fun_decl;
            ast_da_free(&fun_decl.stmts);
        }; break;
        case AST_RETURN_STMT: {
            struct ast_return_stmt ret = ast->as.return_stmt;            
            ast_free(ret.expr);
        }; break;
        case AST_NUMBER: break; // nothing else to free
    }

    free(ast);
}

void _pretty_print(struct ast* ast, int indent) {
    int i;
    for (i = 0; i < indent; ++i) printf("  ");

    switch (ast->tag) {
        case AST_PROGAM: {
            struct ast_program program = ast->as.program;
            puts("PROGRAM:");
            for (i = 0; i < program.decls.len; ++i) {
                _pretty_print(program.decls.at[i], indent+1);
            }
        }; break;
        case AST_FUN_DECL: {
            struct ast_fun_decl fun_decl = ast->as.fun_decl;
            puts("FUN-DECL:");
            for (i = 0; i < indent+1; ++i) printf("  ");
            printf("NAME: %.*s\n", fun_decl.name.len, fun_decl.name.ptr);
            for (i = 0; i < indent+1; ++i) printf("  ");
            puts("BODY:");
            for (i = 0; i < fun_decl.stmts.len; ++i) {
                _pretty_print(fun_decl.stmts.at[i], indent+2);
            }
        }; break;
        case AST_RETURN_STMT: {
            struct ast_return_stmt ret_stmt = ast->as.return_stmt;
            puts("RETURN:");
            _pretty_print(ret_stmt.expr, indent+1);
        }; break;
        case AST_NUMBER: {
            struct ast_number number = ast->as.number;
            puts("NUMBER:");
            for (i = 0; i < indent+1; ++i) printf("  ");
            printf("%d\n", number.n);
        } break;
    }
}

void pretty_print(struct ast* ast) {
    if (!ast) {
        set_gerror(ERROR(ERROR_NULLPTR, "NULL ast passed to pretty_print"));
        return;
    }

    _pretty_print(ast, 0);
}

void femit(FILE* file, struct ast* ast) {
    switch (ast->tag) {
        case AST_PROGAM: {
            fprintf(file, "\
\tsection .text\n\
\tglobal _start\n\
_start:\n\
\tcall   main\n\
\tmov    rdi, rax\n\
\tmov    rax, 60\n\
\tsyscall\n");
            struct ast_program prog = ast->as.program; 
            for (int i = 0; i < prog.decls.len; ++i) {
                femit(file, prog.decls.at[i]);
            }
        } break;
        case AST_FUN_DECL: {
            struct ast_fun_decl fun_decl = ast->as.fun_decl; 
            fprintf(file, "%.*s:\n", fun_decl.name.len, fun_decl.name.ptr);
            for (int i = 0; i < fun_decl.stmts.len; ++i) {
                femit(file, fun_decl.stmts.at[i]);
            }
        } break;
        case AST_RETURN_STMT: {
            struct ast_return_stmt ret = ast->as.return_stmt; 
            fprintf(file, "\tmov rax, ");
            femit(file, ret.expr);
            fprintf(file, "\tret\n");
        } break;
        case AST_NUMBER: {
            struct ast_number number = ast->as.number; 
            fprintf(file, "%d\n", number.n);
        } break;
    }
}
