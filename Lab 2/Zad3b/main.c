#include "explore_nftw.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if(argc != 2){
        printf("Wrong number of arguments!");
        return 0;
    }
    char path[256];
    if(realpath(argv[1], path) == NULL){
        perror("Problem with finding path!");
        return 0;
    }
    traverse_nftw(path);
    return 0;
}
