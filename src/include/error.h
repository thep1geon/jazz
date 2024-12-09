#ifndef __ERROR_H
#define __ERROR_H

struct error {
    enum {
        ERROR_NONE,
        ERROR_SYNTAX,
        ERROR_GENERIC,
        ERROR_NULLPTR,
    } type;

    const char* msg;
};

#define ERROR_OK ((struct error){0, 0})
#define ERROR(t, m) ((struct error){t, m})

void error_print(struct error error);

struct error get_gerror();
void set_gerror(struct error);

#endif  //__ERROR_H
