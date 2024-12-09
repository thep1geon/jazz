#ifndef __STRING_H
#define __STRING_H

#include <stddef.h>
#include <stdbool.h>

struct string {
    char*  ptr;
    int len;
};

#define STRING(p, l) ((struct string){p, l})

struct string string(char* literal);
bool string_eq(struct string a, struct string b);

#endif  //__STRING_H
