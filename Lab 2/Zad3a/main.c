#include "explore_statistics.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Wrong number of arguments!")
        return 0;
    }
    char path[256];
    char *value;
    value = realpath(argv[1], path);
    if (value == NULL) {
        printf("Problem with finding a path!")
        return 0;
    }
    traverse_the_tree(path, 1);
    return 0;
}
