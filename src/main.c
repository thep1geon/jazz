#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/string.h"
#include "include/error.h"
#include "include/ast.h"
#include "include/parser.h"
#include "include/util.h"
#include "include/emit.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        return 64;
    }

    struct string contents = slurpfile(argv[1]);
    if (!contents.ptr) return 3;

    struct ast* ast = parse(contents.ptr);
    if (!ast) {
        error_print(get_gerror());
        goto end;
    }

    pretty_print(ast);

    int len = strlen(argv[1]);
    argv[1][len-3] = 0;
    argv[1][len-4] = 'S';
    FILE* file = fopen(argv[1], "wb");
    femit(file, ast);
    fclose(file);

    char buf[128] = {0};
    snprintf(buf, 128, "nasm -felf64 %s -o a", argv[1]);
    system(buf);
    argv[1][len-5] = 0;
    snprintf(buf, 128, "ld a -o %s", argv[1]);
    system(buf);
    system("rm a");

    ast_free(ast);

end:
    free(contents.ptr);
    return 0;
}
