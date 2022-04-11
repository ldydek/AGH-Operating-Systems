#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/wait.h>

int interrupt_num;


void set_sigaction(int signum, int flags, void *handler, void *action) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = flags;
    if (action == NULL) act.sa_handler = handler;
    else act.sa_sigaction = action;
    sigaction(signum, &act, NULL);
}


void handle_siginfo(int sig_number, siginfo_t *info, void *context) {
    printf("Got a SIGINT signal\n");
    printf("Signal number: %d \n", sig_number);
    printf("Process ID of sending process: %d \n", info->si_pid);
    printf("Parent PID: %d \n", getppid());
    printf("Signal code: %d\n", info->si_code);
    printf("User time of the signal: %ld\n", info->si_utime);
    printf("Signal value: %d \n\n", info->si_value.sival_int);
}

void test1(){
    struct sigaction case1;
    case1.sa_flags = SA_SIGINFO;
    case1.sa_sigaction = handle_siginfo;
    sigemptyset(&case1.sa_mask);

    if (sigaction(SIGUSR1, &case1, NULL) == -1 || sigaction(SIGUSR2, &case1, NULL) == -1 || sigaction(SIGRTMIN, &case1, NULL) == -1){
        exit(1);
    }
    printf("======= TESTING SIGINFO ========\n");
    raise(SIGUSR1);
    raise(SIGUSR2);
    raise(SIGRTMIN);
    printf("\n");
}


void handle_resethand(int sig_no) {
    printf("Got a SIGINT signal with flag SA_RESETHAND \n");
    printf("Resetting signal handling to SIG_DFL \n");
}

void test2() {
    struct sigaction act_info;
    sigemptyset(&act_info.sa_mask);
    act_info.sa_flags = SA_RESETHAND;
    act_info.sa_handler = (__sighandler_t) handle_resethand;

    sigaction(SIGINT, &act_info, NULL);
    printf("***** TESTING SA_RESETHAND ***** \n");
    printf("Raising with the SA_RESETHAND set: \n");

    raise(SIGINT);
    printf("\nRaising with the SA_RESETHAND set for the second time: \n");

    sleep(1);
    signal(SIGINT, handle_resethand);
    raise(SIGINT);
    printf("\n\n");
}


void handle_nocldstop(int sig) {
    printf("Signal SIGCHLD handler, received signal: %d\n", sig);
}

void handle_sa_nodefer(int sig) {
    int im = interrupt_num;
    interrupt_num++;
    printf("Starting handling SA_NODEFER number %d \n", im);
    if(interrupt_num < 5)
        kill(getpid(), SIGINT);
    sleep(1);

    printf("Ending handling SA_NODEFER number %d \n", 5-im-1);
}

void test3() {
    struct sigaction act_info;
    printf("***** TESTING SA_NODEFER ***** \n");
    act_info.sa_flags = SA_NODEFER;
    act_info.sa_handler = &handle_sa_nodefer;
    sigaction(SIGINT, &act_info, NULL);
    raise(SIGINT);
    printf("\n\n");
}

int main(int argc, char **argv) {
    test1();
    test2();
    test3();
    return 0;
}
