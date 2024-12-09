#include "include/error.h"
#include <stdio.h>

struct error g_error = {0};

void error_print(struct error error) {
    if (error.type == ERROR_NONE) return;
    printf("ERROR : ");
    switch (error.type) {
        case ERROR_GENERIC: printf("Generic"); break;
        case ERROR_SYNTAX: printf("Syntax"); break;
        case ERROR_NULLPTR: printf("NULL pointer passed to function"); break;
        case ERROR_NONE: break;
    }

    printf("\n      : %s\n", error.msg);
}

struct error get_gerror() {
    return g_error;
}

void set_gerror(struct error err) {
    g_error = err;
}
