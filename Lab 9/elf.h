#ifndef OPERATING_SYSTEMS_ELF_H
#define OPERATING_SYSTEMS_ELF_H

#include "header.h"

void elf_wait(int id) {
    pthread_mutex_lock(&ewm);
    while (waiting_elves == WORKSHOP_CAPACITY) {
        printf("Elf: czeka na powrot elfow, ID: %d\n", id);
        pthread_cond_wait(&ewc, &ewm);
    }
    pthread_mutex_unlock(&ewm);
}

void elf_report_problem(int id) {
    pthread_mutex_lock(&em);
    if (waiting_elves < WORKSHOP_CAPACITY) {
        elves_queue[waiting_elves] = id;
        waiting_elves++;
        printf("Elf: czeka %d elfów na Mikolaja, ID: %d\n", waiting_elves, id);

        if (waiting_elves == WORKSHOP_CAPACITY){
            printf("Elf: wybudzam Mikolaja, ID: %d\n", id);
            pthread_mutex_lock(&sm);
            pthread_cond_broadcast(&sc);
            pthread_mutex_unlock(&sm);
        }
    }
    pthread_mutex_unlock(&em);
}

void* elf(void* arg){
    int id = *((int *) arg);
    while(true){
        sleep(ELF_WORKING_TIME);
        elf_wait(id);
        elf_report_problem(id);
    }
}

#endif //OPERATING_SYSTEMS_ELF_H
