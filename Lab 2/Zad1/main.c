#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/stat.h>

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms, FILE* file){
    printf("\nEXECUTION TIME\n");
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
    fprintf(file, "\nEXECUTION TIME\n");
    fprintf(file, "real %Lf\n", time_sec(clock_end - clock_start));
    fprintf(file, "user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    fprintf(file, "sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}


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
    char c;
    int number = 1;
    int ctr =  - 1;


    while (len == 0) {
        ctr = -1;
        do {
            number = fread(&c, sizeof(char), 1, file_from);
            ctr++;
            file_line[ctr] = c;
        } while ((c != '\n') && (number == 1));
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
        } while ((c != '\n') && (number == 1));
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

void system_copying(char *first_file, char *second_file) {
    int tab_size = 256;
    int len = 0;
    int ctr = -1;
    char file_line[tab_size];
    char c;
    for (int i=0; i<tab_size; i++) {
        file_line[i] = '\0';
    }

    int file_from = open(first_file, O_RDONLY);
    int file_to = open(second_file, O_RDWR);
    int condition = 1;

    while (len == 0) {
        ctr = -1;
        do {
            condition = read(file_from, &c, sizeof(char));
            ctr++;
            file_line[ctr] = c;
        } while ((c != '\n') && (condition == 1));
        len = line_length(file_line);
        if (len == 0) {
            continue;
        }
        write(file_to, file_line, ctr);
    }


    while (condition == 1) {
        ctr = -1;
        do {
            condition = read(file_from, &c, sizeof(char));
            ctr++;
            file_line[ctr] = c;
        } while ((c != '\n') && (condition == 1));
        len = line_length(file_line);
        if (len == 0) {
            continue;
        }
        write(file_to, "\n", 1);
        write(file_to, file_line, ctr);
    }
    close(file_from);
    close(file_to);
}


int main(int argc, char **argv){
    char *file_from = NULL;
    char *file_to = NULL;

    if (argc == 1 || argc == 0){
        file_from = calloc(256, sizeof(char));
        file_to = calloc(256, sizeof(char));
        printf("Type file names:\n");
        scanf("%s", file_from);
        scanf("%s", file_to);
    }
    else {
        file_from = argv[1];
        file_to = argv[2];
    }

    FILE* result_file = fopen("pomiar_zad_1.txt", "w");
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    struct tms start_tms2;
    struct tms end_tms2;
    clock_t clock_start2;
    clock_t clock_end2;

    printf("\n------------LIB------------\n");
    clock_start = times(&start_tms);
    library_copying(file_from, file_to);
    clock_end = times(&end_tms);

    printf("\n\n------------SYS------------\n");
    clock_start2 = times(&start_tms2);
    system_copying(file_from, file_to);
    clock_end2 = times(&end_tms2);


    fprintf(result_file, "\n------LIB-----");
    printf("\n------LIB-----");
    print_res(clock_start, clock_end, start_tms, end_tms,result_file);

    fprintf(result_file, "\n------SYS-----");
    printf("\n------SYS-----");
    print_res(clock_start2, clock_end2, start_tms2, end_tms2, result_file);
}
