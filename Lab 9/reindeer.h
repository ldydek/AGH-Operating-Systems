#ifndef OPERATING_SYSTEMS_RAINDEER_H
#define OPERATING_SYSTEMS_RAINDEER_H

#include "header.h"

void reindeer_wait() {
    pthread_mutex_lock(&rwm);
    while (waiting_reindeer != 0)
        pthread_cond_wait(&rwc, &rwm);
    pthread_mutex_unlock(&rwm);
}

void reindeer_deliver_presents(int id) {
    pthread_mutex_lock(&rm);
    waiting_reindeer++;
    printf("Renifer: czeka %d reniferow na Mikolaja, %d\n", waiting_reindeer, id);
    if (waiting_reindeer == REINDEER_NUM) {
        printf("Renifer: wybudzam Mikolaja, %d\n", id);
        pthread_mutex_lock(&sm);
        pthread_cond_broadcast(&sc);
        pthread_mutex_unlock(&sm);
    }
    pthread_mutex_unlock(&rm);
}

void* reindeer(void* arg){
    int id = *((int *) arg);
    while(true){
        reindeer_wait();
        sleep(REINDEER_HOLIDAY_TIME);
        reindeer_deliver_presents(id);
    }
}
#endif
