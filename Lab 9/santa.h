#ifndef OPERATING_SYSTEMS_SANTA_H
#define OPERATING_SYSTEMS_SANTA_H
#include "header.h"

void santa_sleep() {
    pthread_mutex_lock(&sm);
    while (waiting_elves < WORKSHOP_CAPACITY && waiting_reindeer < REINDEER_NUM)
        pthread_cond_wait(&sc, &sm);
    pthread_mutex_unlock(&sm);
}

void santa_deliver_presents() {
    printf("Mikolaj: Budze sie.\n");
    deliveries++;
    printf("Mikolaj: dostarczam zabawki [licznik dostaw: %d]\n", deliveries);
    sleep(DELIVERING_TIME);

    pthread_mutex_lock(&rwm);
    waiting_reindeer = 0;
    pthread_cond_broadcast(&rwc);
    pthread_mutex_unlock(&rwm);
}

void santa_solve_elf_problems() {
    printf("Mikolaj: Budze sie.\n");
    printf("Mikolaj: rozwiazuje problemy elfow [%d][%d][%d]\n", elves_queue[0], elves_queue[1], elves_queue[2]);
    for (int i = 0; i < 3; i++)
        printf("Elf: Mikolaj rozwiazuje problem, ID: %d\n", elves_queue[i]);
    sleep(SANTA_SOLVING_TIME);

    pthread_mutex_lock(&ewm);
    reset_queue(elves_queue);
    waiting_elves = 0;
    pthread_cond_broadcast(&ewc);
    pthread_mutex_unlock(&ewm);
}

void *santa(){
    while (true){
        santa_sleep();

        pthread_mutex_lock(&rm);
        if (waiting_reindeer == REINDEER_NUM)
            santa_deliver_presents();
        pthread_mutex_unlock(&rm);

        if (deliveries == 3) {
            break;
        }

        pthread_mutex_lock(&em);
        if (waiting_elves == WORKSHOP_CAPACITY)
            santa_solve_elf_problems();
        pthread_mutex_unlock(&em);

        printf("Mikolaj: zasypiam\n");
    }
    exit(0);
}
#endif
