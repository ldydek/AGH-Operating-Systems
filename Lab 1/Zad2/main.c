#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "library.h"
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <sys/resource.h>

clock_t st_time, en_time;
struct tms st_cpu, en_cpu;
char* report_file_name = "raport2.txt";


void write_report_header(FILE *f){
    printf("%40s\t\t%15s\t%15s\t%15s\n",
            "Name",
            "Real [s]",
            "User [s]",
            "System [s]");
}

void print_table(struct table *tablica) {
    for (int i=0; i<tablica->size; i++) {
        printf("%p\n", tablica->arr[i]);
    }
}

int command(char **commands, char *word, int commands_length) {
    for (int i=0; i<commands_length; i++) {
        if (strcmp(commands[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

void save_timer(char *name, FILE *f){
    int clk_tics = sysconf(_SC_CLK_TCK);
    double real_time = (double)(en_time - st_time) / clk_tics;
    double user_time = (double)(en_cpu.tms_utime - st_cpu.tms_utime) / clk_tics;
    double system_time = (double)(en_cpu.tms_stime - st_cpu.tms_stime) / clk_tics;
    printf("%40s:\t\t%15f\t%15f\t%15f\t\n",
            name,
            real_time,
            user_time,
            system_time);
    fprintf(f, "%40s:\t\t%15f\t%15f\t%15f\t\n",
            name,
            real_time,
            user_time,
            system_time);
}


int main(int argc, char** argv) {
    FILE* report_file = fopen(report_file_name, "a");
    write_report_header(report_file);

    struct table *tablica = NULL;
    char **file_names = NULL;
    char* commands[] = {"create_table", "wc_files", "read_data_from_file", "remove_block", "start_timer",
     "stop_timer", "save_timer", "remove_table", "create_block"};
    int commands_size = 8;

    int ctr = 1;
    while (ctr < argc) {
        if (!(command(commands, argv[ctr], commands_size))) {
            ctr++;
            continue;
        }
        if (strcmp(argv[ctr], "create_table") == 0) {
            ctr++;
            if (ctr < argc) {
                int table_size = atoi(argv[ctr]);
                tablica = create_table(tablica, table_size);
                ctr++;
            }
        }
        else if (strcmp(argv[ctr], "wc_files") == 0) {
            ctr++;
            int files_number = 0;
            int start = ctr;
            while (ctr < argc && !(command(commands, argv[ctr], commands_size))) {
                files_number++;
                ctr++;
            }
            file_names = (char **) calloc(files_number, sizeof(char *));
            for (int i=0; i<files_number; i++) {
                file_names[i] = argv[start+i];
            }
            wc_files_to_temp_file(file_names, files_number);
            free(file_names);
            file_names = NULL;
        }
        else if (strcmp(argv[ctr], "read_data_from_file") == 0) {
            ctr++;
            read_data_from_file(tablica);
        }
        else if (strcmp(argv[ctr], "remove_block") == 0) {
            ctr++;
            if ((ctr < argc) && ((tablica != NULL) && !(command(commands, argv[ctr], commands_size)))) {
                int block_number = atoi(argv[ctr]);
                remove_block(tablica, block_number);
                ctr++;
            }
        }
        else if (strcmp(argv[ctr], "remove_table") == 0) {
            ctr++;
            remove_table(tablica);
        }
        else if (strcmp(argv[ctr], "save_timer") == 0) {
            ctr++;
            if (ctr < argc) {
                save_timer(argv[ctr], report_file);
            }
        }
        else if (strcmp(argv[ctr], "start_timer") == 0) {
            ctr ++;
            st_time = times(&st_cpu);
        }
        else if (strcmp(argv[ctr], "stop_timer") == 0) {
            ctr++;
            en_time = times(&en_cpu);
        }
        else if (strcmp(argv[ctr], "create_block") == 0) {
            ctr++;
            if (ctr < argc) {
                int block_number = atoi(argv[ctr]);
                int k = 5;
                create_block(tablica, k, block_number);
                ctr++;
            }
        }
    }
    if (tablica != NULL) {
        remove_table(tablica);
    }
    return 0;
}
