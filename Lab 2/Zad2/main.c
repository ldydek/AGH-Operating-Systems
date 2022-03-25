#include <stdio.h>
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

int counting_signes(char line[], char sign) {
    int ctr = 0, ctr1 = 0;
    while ((line[ctr] != '\n')  && (line[ctr] != '\0')) {
        if (line[ctr] == sign) {
           ctr1++;
        }
        ctr++;
    }
    return ctr1;
}

void library_checking(char sign, char *file1, int solution[]) {
    int tab_size = 256;
    int number = 1;
    int ctr;
    char c;
    char file_line[tab_size];
    int len = 0;
    for (int i=0; i<tab_size; i++) {
        file_line[i] = '\0';
    }
    FILE *file = fopen(file1, "r");

    while (number == 1) {
        ctr = -1;
        do {
            number = fread(&c, sizeof(char), 1, file);
            ctr++;
            file_line[ctr] = c;
        } while ((c != '\n') && (number == 1));
        len = counting_signes(file_line, sign);
        if (len != 0) {
            solution[0] += len;
            solution[1]++;
        }
    }
    fclose(file);
}

void system_checking(char sign, char *file1, int solution[]) {
    int tab_size = 256;
    int len = 0;
    int ctr = -1;
    char file_line[tab_size];
    char c;
    for (int i=0; i<tab_size; i++) {
        file_line[i] = '\0';
    }
    int file = open(file1, O_RDONLY);
    int condition = 1;

    while (condition == 1) {
        ctr = -1;
        do {
            condition = read(file, &c, sizeof(char));
            ctr++;
            file_line[ctr] = c;
        } while ((c != '\n') && (condition == 1));
        len = counting_signes(file_line, sign);
        if (len != 0) {
            solution[0] += len;
            solution[1]++;
        }
    }
    close(file);
}

int main(int argc, char **argv) {
    int solution[2] = {0, 0};
    char *sign = NULL;
    char *file = NULL;
    sign = argv[1];
    file = argv[2];

    FILE* result_file = fopen("pomiar_zad_2.txt", "w");
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    struct tms start_tms2;
    struct tms end_tms2;
    clock_t clock_start2;
    clock_t clock_end2;

    clock_start = times(&start_tms);
    library_checking(*sign, file, solution);
    clock_end = times(&end_tms);

    for (int i=0; i<2; i++) {
        printf("%d ", solution[i]);
    }

    for (int i=0; i<2; i++) {
        solution[i] = 0;
    }

    clock_start2 = times(&start_tms2);
    system_checking(*sign, file, solution);
    clock_end2 = times(&end_tms2);


    fprintf(result_file, "\n------LIB-----");
    printf("\n------LIB-----");
    print_res(clock_start, clock_end, start_tms, end_tms,result_file);

    for (int i=0; i<2; i++) {
        printf("%d ", solution[i]);
    }

    fprintf(result_file, "\n------SYS-----");
    printf("\n------SYS-----");
    print_res(clock_start2, clock_end2, start_tms2, end_tms2, result_file);
}
