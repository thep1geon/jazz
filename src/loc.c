#include "include/loc.h"
#include <stdio.h>

struct location location(int line, int column) {
    return (struct location){line, column};
}

void location_print(struct location loc) {
    printf("(LOCATION){line: %d, column: %d}\n", loc.line, loc.column);
}
