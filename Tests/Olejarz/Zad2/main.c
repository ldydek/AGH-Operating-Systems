#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int main() {
    pid_t child;
    int status, retval;
    if ((child = fork()) < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (child == 0) {
        // child
        sleep(1);
        exit(EXIT_SUCCESS);
    } else {
/* Proces macierzysty pobiera status  zakoñczenie potomka child,
 * nie zawieszaj¹c swojej pracy. Jeœli proces siê nie zakoñczy³, wysy³a do dziecka sygna³ SIGKILL.
 * Jeœli wys³anie sygna³u siê nie powiod³o, ponownie oczekuje na zakoñczenie procesu child,
 * tym razem zawieszaj¹c pracê do czasu zakoñczenia sygna³u
 * jeœli siê powiod³o, wypisuje komunikat sukcesu zakoñczenia procesu potomka z numerem jego PID i statusem zakoñczenia. */

    // parent
   waitpid(child, &status, WNOHANG);
    if (!WIFEXITED(status)) {
        // wysylanie sygnalu
        if (kill(child, SIGKILL) < 0) {
             wait(&status);
        }
        else {
            printf("Child process terminated with PID: %d and status: %d\n", child, WEXITSTATUS(status));
        }
    }
    // CHYBA DONE
/* koniec*/
    } //else
    exit(EXIT_SUCCESS);
}
