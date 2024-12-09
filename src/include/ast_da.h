#ifndef __AST_DA_H
#define __AST_DA_H

struct ast_da {
    struct ast** at;
    int len;
    int cap;
};

struct ast_da ast_da_alloc(int initial_cap);
void ast_da_free(struct ast_da* da);
void ast_da_append(struct ast_da* da, struct ast* ast);

#endif  //__AST_DA_H
