#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


int line_length(char line[]) {
    int ctr = 0;
    int sol = 0;
    while ((line[ctr] != '\n')  && (line[ctr] != '\0')) {
        if ((line[ctr] != '\t') && (line[ctr] != '\r') && ((int) line[ctr] != 32)) {
            sol++;
        }
        ctr++;
    }
    return sol;
}

void library_copying(char *first_file, char *second_file) {
    int tab_size = 256;
    int len = 0;
    char file_line[tab_size];
    for (int i=0; i<tab_size; i++) {
        file_line[i] = '\0';
    }
    FILE *file_from = fopen(first_file, "r");
    FILE *file_to = fopen(second_file, "w+");
    int line_len = 0;
    char c = 'a';
    int number = 1;
    int ctr =  - 1;


    while (len == 0) {
        ctr = -1;
        do {
            fread(&c, sizeof(char), 1, file_from);
            ctr++;
            file_line[ctr] = c;
        } while (c != '\n');
        len = line_length(file_line);
        if (len == 0) {
            continue;
        }
        fwrite(file_line, 1, ctr, file_to);
    }

    while (number == 1) {
        ctr = -1;
        do {
            number = fread(&c, sizeof(char), 1, file_from);
            ctr++;
            file_line[ctr] = c;
        } while (c != '\n');
        len = line_length(file_line);
        if (len == 0) {
            continue;
        }
        fwrite("\n", 1, 1, file_to);
        fwrite(file_line, 1, ctr, file_to);
    }
    fclose(file_from);
    fclose(file_to);
}


int main(int argc, char **argv) {
    library_copying("file_from.txt", "file_to.txt");
}
