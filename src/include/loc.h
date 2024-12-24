#ifndef __LOC_H
#define __LOC_H

struct location {
    int line;
    int column;
};

struct location location(int line, int column);

void location_print(struct location);

#endif  //__LOC_H
