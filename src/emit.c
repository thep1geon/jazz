#include <assert.h>
#include <stdio.h>

#include "include/emit.h"
#include "include/ast.h"

void emit_ast_program(FILE* file, struct ast_program ast) {
    fprintf(file, "\tsection .text\n\tglobal _start\n_start:\n\tcall   main\n\tmov    rdi, rax\n\tmov    rax, 60\n\tsyscall\n");
    for (int i = 0; i < ast.decls.len; ++i) {
        femit(file, ast.decls.at[i]);
    }
}

void emit_ast_fun_decl(FILE* file, struct ast_fun_decl ast) {
    fprintf(file, "%.*s:\n", ast.name.len, ast.name.ptr);
    fprintf(file, "\tpush rbp\n\tmov rbp, rsp\n");
    for (int i = 0; i < ast.stmts.len; ++i) {
        if (i == ast.stmts.len - 1) {
            fprintf(file, "\tpop rbp\n");
        }
        femit(file, ast.stmts.at[i]);
    }
}

void emit_ast_return_stmt(FILE* file, struct ast_return_stmt ast) {
    femit(file, ast.expr);
    fprintf(file, "\tret\n");
}

void emit_ast_number(FILE* file, struct ast_number ast) {
    fprintf(file, "\tmov eax, %d\n", ast.n);
}

void emit_ast_unary_op(FILE* file, struct ast_unary_op ast) {
    struct token tok = ast.tok; 

    switch (tok.type) {
        case TOK_DASH: {
            femit(file, ast.expr);
            fprintf(file, "\tneg eax\n");
        } break;
        case TOK_TILDA: {
            femit(file, ast.expr);
            fprintf(file, "\tnot eax\n");
        } break;
        case TOK_BANG: {
            femit(file, ast.expr);
            fprintf(file, "\tcmp eax, 0\n");
            fprintf(file, "\tsete al\n");
        } break;
        default: assert(0 && "Unreachable");
    }
}

void femit(FILE* file, struct ast* ast) {
    switch (ast->tag) {
        case AST_PROGAM: {
            emit_ast_program(file, ast->as.program);
        } break;
        case AST_FUN_DECL: {
            emit_ast_fun_decl(file, ast->as.fun_decl);
        } break;
        case AST_RETURN_STMT: {
            emit_ast_return_stmt(file, ast->as.return_stmt);
        } break;
        case AST_NUMBER: {
            emit_ast_number(file, ast->as.number);
        } break;
        case AST_UNARY_OP: {
            emit_ast_unary_op(file, ast->as.unary_op);
        } break;
    }
}
