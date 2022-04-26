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
/* Proces macierzysty pobiera status  zako�czenie potomka child,
 * nie zawieszaj�c swojej pracy. Je�li proces si� nie zako�czy�, wysy�a do dziecka sygna� SIGKILL.
 * Je�li wys�anie sygna�u si� nie powiod�o, ponownie oczekuje na zako�czenie procesu child,
 * tym razem zawieszaj�c prac� do czasu zako�czenia sygna�u
 * je�li si� powiod�o, wypisuje komunikat sukcesu zako�czenia procesu potomka z numerem jego PID i statusem zako�czenia. */

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
