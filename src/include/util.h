#ifndef __UTIL_H
#define __UTIL_H

#include "string.h"

#define LEN(xs) (sizeof((xs))/sizeof((xs)[0]))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a) ((a) > 0 ? (a) : -1 * (a))

struct string slurpfile(const char* filepath);
void print_usage();

#endif  //__UTIL_H
