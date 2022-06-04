#include "header.h"

int oven_shm_id = 0;
int table_shm_id = 0;
int sem_set_id = 0;
struct oven *oven1;
struct table *table1;
struct sembuf sembuf;
char current_time[100];

int main(int argc, char **argv) {
    oven_shm_id = get_oven_shm_id();
    table_shm_id = get_table_shm_id();
    sem_set_id = get_sem_id();

    // wskaźnik na adres pamięci wspólnej
    oven1 = shmat(oven_shm_id, NULL, 0);
    table1 = shmat(table_shm_id, NULL, 0);

    while(1) {
        add_val_to_sem(sem_set_id, EMPTY_TABLE_SEM_ID, sembuf, -1);
        add_val_to_sem(sem_set_id, TABLE_ACCESS_SEM_ID, sembuf, -1);

        int table_pizzas_num = table1 -> pizzas_number;

        int my_pizza_id = (table1 -> next_id - table_pizzas_num + TABLE_SIZE) % TABLE_SIZE;
        if (table1 -> next_id + 1 >= TABLE_SIZE) {
            table1 -> next_id = 0;
        }
        else {
            table1 -> next_id++;
        }
        table1 -> pizzas_number--;
        int my_pizza_type = table1 -> pizzas[my_pizza_id];

        add_val_to_sem(sem_set_id, FULL_TABLE_SEM_ID, sembuf, 1);
        add_val_to_sem(sem_set_id, OVEN_ACCESS_SEM_ID, sembuf, -1);

        current_timestamp(current_time);
        printf("PID: %d: %s: Pobieram pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d\n", getpid(),
               current_time, my_pizza_type, oven1 -> pizzas_number, table1 -> pizzas_number);

        add_val_to_sem(sem_set_id, TABLE_ACCESS_SEM_ID, sembuf, 1);
        add_val_to_sem(sem_set_id, OVEN_ACCESS_SEM_ID, sembuf, 1);

        sleep(TRAVEL_TIME);

        current_timestamp(current_time);
        printf("PID: %d: %s: Dostarczam pizze: %d\n", getpid(),
               current_time, my_pizza_type);

        sleep(TRAVEL_TIME);
    }
}
