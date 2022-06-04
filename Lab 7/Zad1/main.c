#include "header.h"


int oven_shm_id = 0;
int table_shm_id = 0;
int semaphore_set_id = 0;
struct oven *oven1;
struct table *table1;

void exit_procedure(){
    printf("Exit procedure\n");
    shmdt(oven1);
    shmdt( table1);
    shmctl(oven_shm_id, IPC_RMID, NULL);
    shmctl(table_shm_id, IPC_RMID, NULL);
    semctl(semaphore_set_id, 0, IPC_RMID, NULL);
}

// tworzenie pamieci wspó³dzielonej
void create_shm(){
    key_t oven_key = ftok(get_home_path(), OVEN_PROJ_ID);
    key_t table_key = ftok(get_home_path(), TABLE_PROJ_ID);

    oven_shm_id = shmget(oven_key, sizeof(struct oven), IPC_CREAT | 0666);
    table_shm_id = shmget(table_key, sizeof(struct table), IPC_CREAT | 0666);

    oven1 = shmat(oven_shm_id, NULL, 0);
    table1 = shmat(table_shm_id, NULL, 0);

    printf("Created oven (id: %d) and table (id: %d)\n", oven_shm_id, table_shm_id);
}

// tworzenie zbioru semaforów
void create_sem(){
    key_t sem_key = ftok(get_home_path(), SEM_PROJ_ID);
    semaphore_set_id = semget(sem_key, 5, IPC_CREAT | 0666);
    printf("Semaphore set created (id: %d)\n", semaphore_set_id);
}

void initialize_sem(){
    union semun arg;
    arg.val = 1;

    semctl(semaphore_set_id, OVEN_ACCESS_SEM_ID, SETVAL, arg);
    semctl(semaphore_set_id, TABLE_ACCESS_SEM_ID, SETVAL, arg);

    arg.val = OVEN_SIZE;
    semctl(semaphore_set_id, FULL_OVEN_SEM_ID, SETVAL, arg);

    arg.val = TABLE_SIZE;
    semctl(semaphore_set_id, FULL_TABLE_SEM_ID, SETVAL, arg);

    arg.val = 0;
    semctl(semaphore_set_id, EMPTY_TABLE_SEM_ID, SETVAL, arg);
}

int main(int argc, char **argv){
    if(argc != 3){
        printf(stderr, "Wrong number of arguments!!!");
        return 0;
    }
    char *a = argv[1];
    char *b = argv[2];
    int bakers_number = atoi(a);
    int deliverers_number = atoi(b);

    create_shm();

    oven1 -> next_id = 0;
    oven1 -> pizzas_number = 0;

    table1 -> next_id = 0;
    table1 -> pizzas_number = 0;

    create_sem();
    initialize_sem();

    signal(SIGINT, exit_procedure);

    for(int i=0; i<bakers_number; i++){
        if(fork() == 0){
            execl("./baker", "./baker", NULL);
        }
    }

    for(int i=0; i<deliverers_number; i++){
        if(fork() == 0){
            execl("./deliverer", "./deliverer", NULL);
        }
    }

    while(1){
        sleep(1);
    }
}
