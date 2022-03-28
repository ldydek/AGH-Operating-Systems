#include <stdlib.h>
#include "malloc.h"
#include "unistd.h"
#include "string.h"
#include <sys/times.h>
#include <stdint.h>
#include <sys/wait.h>


long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t start_timer, clock_t stop_timer, struct tms start_tms, struct tms end_tms){
    printf("real %Lf\n", time_sec(stop_timer - start_timer));
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}

double f(double x){
    return 4/(x*x + 1);
}

double surface_area(double from , double to){
    return f((from+to)/2.0) * (to-from);
}

void save_to_file(int counter, double result){
    char *k = calloc(sizeof(char), 256);
    sprintf(k, "w%d.txt", counter);
    FILE *destination_file = fopen(k, "wr");
    fprintf(destination_file, "%f", result);
    free(k);
    fclose(destination_file);
}

void count_range(int i, int number_of_processes, double rectangle_width){
    double i1 = (double) i;
    double number = (double) number_of_processes;
    double left = ((i1-1)/number);
    double right= (i1/number);
    double result = 0.0;
    while (left + rectangle_width <= right) {
        result += surface_area(left, left + rectangle_width);
        left += rectangle_width;
    }
    result += surface_area(left, right);
    save_to_file(i, result);
}


int main(int argc, char **argv) {
    double width_of_rectangle;
    int number_of_processes;
    struct tms start_tms;
    struct tms end_tms;
    clock_t start_timer;
    clock_t stop_timer;

    if (argc < 3){
        printf("Too few arguments!");
        return -1;
    }
    else {
        width_of_rectangle = atof(argv[1]);
        number_of_processes = atoi(argv[2]);
    }
    start_timer = times(&start_tms);

    int counter = 1;
    int index = 0;
    int process_pid = 0;
    while (counter <= number_of_processes) {
        process_pid = fork();
        index++;
        if (process_pid == 0) {
            count_range(counter, number_of_processes, width_of_rectangle);
            break;
        }
        else {
            counter++;
        }
    }
    if (process_pid != 0) {
        pid_t wait_pid;
        int status = 0;
        for (int i = 0; i < index; i++) {
            while ((wait_pid = wait(&status)) > 0);
        }

        double whole_result = 0;
        for (int i=1; i <= number_of_processes; i++ ) {
            char file_name[10];
            sprintf(file_name, "w%d.txt", i);
            FILE* file = fopen(file_name, "rw");
            size_t buffer_size = 32;
            char buffer[buffer_size];
            fread(buffer, sizeof(char), buffer_size, file);
            whole_result += atof(buffer);
            fclose(file);
        }
        printf("result: %f\nwidth_of_rectangle: %f\nnumber of processes: %d\n", whole_result, width_of_rectangle, number_of_processes);
        stop_timer = times(&end_tms);
        print_res(start_timer, stop_timer, start_tms, end_tms);
    }
    return 0;
}
