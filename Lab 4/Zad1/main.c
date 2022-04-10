#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include "stdio.h"

void raise_signal() {
    raise(SIGUSR1);
    pid_t pid = fork();
    if (pid == 0){
        printf("Raising child signal \n");
        raise(SIGUSR1);
    }
    else {
        wait(NULL);
    }
}

void handler(int sigint){
    printf("SIGUSR1 received\n");
}

void pending(int sig){
    sigset_t set;
    if(sigpending(&set) == -1){
        fprintf(stderr, "Pending signals could not be obtained: %s\n", strerror(errno));
        exit(1);
    }
    if (sigismember(&set, sig)){
        printf("signal %d is pending\n", sig);
    }
    else {
        printf("signal %d is not pending\n", sig);
    }
}

void mask(int sig){
    sigset_t new_mask;
    sigemptyset(&new_mask);
    sigaddset(SIG_BLOCK, sig);
    if (sigprocmask(SIG_BLOCK, &new_mask, NULL) < 0){
        fprintf(stderr, "Signal could not be blocked: %s\n", strerror(errno));
        exit(1);
    }
}

int main(int argc, char** argv){
    if (argc <= 1){
        printf("Too little arguments!");
        return 0;
    }
    int sig = SIGUSR1;
    char *action = argv[1];
    if (strcmp(action, "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN);
        raise_signal();
    }
    else if (strcmp(action, "handler") == 0) {
        signal(SIGUSR1, handler);
        raise_signal();
    }
    else if (strcmp(action, "pending") == 0) {
        pending(SIGUSR1);
    }
    else if (strcmp(action, "mask") == 0) {
        mask(SIGUSR1);
    }
    pid_t kid;
    kid = fork();
    if (kid == -1){
        fprintf(stderr, "cannot make a child: %s \n", strerror(errno));
    }
    else if (kid == 0) {
        if (strcmp(action, "pending") == 0) {
            pending(SIGUSR1);
        }
        else {
            raise(SIGUSR1);
        }
        printf ("forked sucesfully\n");
        exit(0);
    }
    execl(argv[0], "exec_test", argv[1], NULL);
    return 0;
}
