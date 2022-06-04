#include "header.h"

int oven_shm_id = 0;
int table_shm_id = 0;
int sem_set_id = 0;
struct oven *oven1;
struct table *table1;
struct sembuf sembuf;
char current_time[100];


int main(int argc, char **argv){
    oven_shm_id = get_oven_shm_id();
    table_shm_id = get_table_shm_id();
    sem_set_id = get_sem_id();
    oven1 = shmat(oven_shm_id, NULL, 0);
    table1 = shmat(table_shm_id, NULL, 0);

    srand(getpid());
    while(1) {
        current_timestamp(current_time);
        int pizza = rand() % 10;
        printf("PID: %d: %s: Przygotowuje pizze: %d\n", getpid(), current_time, pizza);

        sleep(PIZZA_PREPARATION);

        add_val_to_sem(sem_set_id, FULL_OVEN_SEM_ID, sembuf, -1);
        add_val_to_sem(sem_set_id, OVEN_ACCESS_SEM_ID, sembuf, -1);
        int my_pizza_id = oven1 -> next_id;
        if (oven1 -> next_id + 1 >= OVEN_SIZE) {
            oven1 -> next_id = 0;
        }
        else {
            oven1 -> next_id++;
        }
        oven1 -> pizzas[my_pizza_id] = pizza;
        oven1 -> pizzas_number++;

        current_timestamp(current_time);
        printf("PID: %d: %s: Dodalem pizze: %d. Liczba pizz w piecu: %d\n", getpid(), current_time, pizza,
               oven1 -> pizzas_number);

        add_val_to_sem(sem_set_id, OVEN_ACCESS_SEM_ID, sembuf, 1);

        sleep(PIZZA_BAKING);

        add_val_to_sem(sem_set_id, OVEN_ACCESS_SEM_ID, sembuf, -1);

        oven1 -> pizzas_number--;
        int oven_pizzas_num = oven1 -> pizzas_number;

        add_val_to_sem(sem_set_id, FULL_OVEN_SEM_ID, sembuf, 1);
        add_val_to_sem(sem_set_id, OVEN_ACCESS_SEM_ID, sembuf, 1);

        add_val_to_sem(sem_set_id, FULL_TABLE_SEM_ID, sembuf, -1);
        add_val_to_sem(sem_set_id, TABLE_ACCESS_SEM_ID, sembuf, -1);

        my_pizza_id = table1 -> next_id;
        if (table1 -> next_id + 1 >= TABLE_SIZE) {
            table1 -> next_id = 0;
        }
        else {
            table1 -> next_id++;
        }
        table1 -> pizzas[my_pizza_id] = pizza;
        table1 -> pizzas_number++;
        int table_pizzas_num = table1 -> pizzas_number;

        current_timestamp(current_time);
        printf("PID: %d: %s: Wyjąlem pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d\n", getpid(),
               current_time, pizza, oven_pizzas_num, table_pizzas_num);

        add_val_to_sem(sem_set_id, EMPTY_TABLE_SEM_ID, sembuf, 1);
        add_val_to_sem(sem_set_id, TABLE_ACCESS_SEM_ID, sembuf, 1);
    }
}
