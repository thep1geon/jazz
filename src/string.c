#include <string.h>

#include "include/string.h"

struct string string(char* literal) {
    return STRING(literal, strlen(literal));
}

bool string_eq(struct string a, struct string b) {
    if (a.len != b.len) return false;

    for (int i = 0; i < (int)a.len; ++i) {
        if (a.ptr[i] != b.ptr[i]) return false;
    }

    return true;
}
