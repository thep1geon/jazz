#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "include/util.h"

struct string slurpfile(const char* filepath) {
    FILE* fd = fopen(filepath, "rb");
    if (!fd) return (struct string){0};

    fseek(fd, 0, SEEK_END);
    size_t filesize = ftell(fd);
    rewind(fd);

    char* buf = malloc(filesize + 1);
    if (!buf) return (struct string){0};

    assert(fread(buf, 1, filesize, fd) == filesize);
    buf[filesize] = 0;

    fclose(fd);
    return (struct string){buf, filesize};
}

void print_usage() {
    printf("USAGE: jazzc [input-file].jazz\n");
}

