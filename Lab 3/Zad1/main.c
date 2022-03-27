#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void creating_processes(int number_of_processes) {
    for (int i=0; i<number_of_processes; i++) {
        pid_t child_pid;
        printf("PID glownego programu: %d\n", (int)getpid());
        child_pid = fork();
        if(child_pid!=0) {
            printf("Proces rodzica: Proces rodzica ma pid:%d\n", (int)getpid());
            printf("Proces rodzica: Proces dziecka ma pid:%d\n", (int)child_pid);
        }
        else {
            printf("Proces dziecka: Proces rodzica ma pid:%d\n",(int)getppid());
            printf("Proces dziecka: Proces dziecka ma pid:%d\n",(int)getpid());
        }
    }
}

int main(int argc, char **argv) {
    int number_of_processes = atoi(argv[1]);
    creating_processes(number_of_processes);
    return 0;
}
