#ifndef CW07_HEADER_H
#define CW07_HEADER_H

#include <pwd.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "stdio.h"
#include "stdlib.h"
#include "sys/sem.h"
#include "sys/ipc.h"
#include "sys/types.h"
#include "sys/shm.h"
#include "signal.h"
#include "sys/wait.h"
#include <sys/prctl.h>

#define TABLE_SIZE 5
#define OVEN_SIZE 5

#define PIZZA_PREPARATION 1
#define TRAVEL_TIME 5
#define PIZZA_BAKING 4

#define OVEN_PROJ_ID 2115
#define TABLE_PROJ_ID 2137
#define SEM_PROJ_ID 2905

#define OVEN_ACCESS_SEM_ID 0
#define TABLE_ACCESS_SEM_ID 1
#define FULL_OVEN_SEM_ID 2
#define FULL_TABLE_SEM_ID 3
#define EMPTY_TABLE_SEM_ID 4



struct table{
    int pizzas[TABLE_SIZE];
    int pizzas_number;
    int next_id;
};

struct oven {
    int pizzas[OVEN_SIZE];
    int pizzas_number;
    int next_id;
};

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

char* get_home_path(){
    char* path = getenv("HOME");
    return path;
}

// funkcje zwracaj¹ce id segmentu pamiêci wspólnej
int get_oven_shm_id(){
    char *home_path = get_home_path();
    key_t oven_key = ftok(home_path, OVEN_PROJ_ID);
    return shmget(oven_key, sizeof(struct oven), 0);
}

int get_table_shm_id(){
    char *home_path = get_home_path();
    key_t table_key = ftok(home_path, TABLE_PROJ_ID);
    return shmget(table_key, sizeof(struct table), 0);
}

// funkcja zwracaj¹ca id zbioru semaforów
int get_sem_id(){
    char *home_path = get_home_path();
    key_t sem_key = ftok(home_path, SEM_PROJ_ID);
    return semget(sem_key, 5, 0);
}

void current_timestamp(char *buffer){
    struct timeval tp;
    gettimeofday(&tp, 0);
    time_t curtime = tp.tv_sec;
    struct tm *t = localtime(&curtime);
//    printf("%02d:%02d:%02d:%03d\n", t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
    sprintf(buffer, "%02d:%02d:%02d:%03ld", t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
}

void add_val_to_sem(int sem_set_id, int sem_id, struct sembuf sembuf, int val){
    sembuf.sem_num = sem_id;
    sembuf.sem_op = val;
    semop(sem_set_id, &sembuf, 1);
}

#endif
