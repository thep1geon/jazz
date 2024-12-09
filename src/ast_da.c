#include "include/ast_da.h"
#include "include/ast.h"
#include <stdlib.h>

struct ast_da ast_da_alloc(int initial_cap) {
    struct ast_da da = {0};
    da.cap = initial_cap;
    da.at = calloc(da.cap, sizeof(struct ast*));
    return da;
}

void ast_da_free(struct ast_da* da) {
    if (!da) return;

    for (int i = 0; i < da->len; ++i) {
        ast_free(da->at[i]);
    }
    free(da->at);
    da->len = 0;
    da->cap = 0;
}

void ast_da_append(struct ast_da* da, struct ast* ast) {
    if (!da) return;

    if (da->len == da->cap) {
        da->cap = (da->cap == 0 ? 8 : da->cap * 2);
        da->at = realloc(da->at, sizeof(struct ast*) * da->cap);
    }

    da->at[da->len++] = ast;
}
