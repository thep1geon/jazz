#ifndef __ERROR_H
#define __ERROR_H

#include "loc.h"

struct error {
    enum {
        ERROR_NONE,
        ERROR_SYNTAX,
        ERROR_GENERIC,
        ERROR_NULLPTR,
    } type;

    const char* msg;

    struct location loc;
};


#define ERROR_OK ((struct error){0, 0, (struct location){0}})
#define ERROR(t, m, l) ((struct error){t, m, l})

void error_print(struct error error);

struct error gerror();
void set_gerror(struct error);

#endif  //__ERROR_H
