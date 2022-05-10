#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <pwd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "header.h"
#define ID 'S'

struct client clients[10];
int current_id = 1;
int server_queue_id;


void send_message(int queue_id, struct message* message_){
    if(msgsnd(queue_id, message_, sizeof(struct message) - sizeof(long), 0) == -1){
        printf("cannot send this message\n");
        exit(1);
    }
}

void receive_message(int queue_id, struct message* message_, long type){
    if(msgrcv(queue_id, message_, sizeof(struct message) - sizeof(long), message_->mode, 0) == -1){
        printf("cannot receive message\n");
        exit(1);
    }
}

void delete_server(int queue){
    msgctl(queue, IPC_RMID, NULL);
}

char *get_path(){
    char *path = getenv("HOME");
    if (path == NULL){
        path = getpwuid(getuid()) -> pw_dir;
    }
    return path;
}

void handle_SIGINT(){
    printf("\nserver received SIGINT\n");
    exit(0);
}

void stop_server(int server_queue_id){
    printf("stopping server\n");
    struct message message_;
    int client_queue;
    for (int i = 0; i < 10; i++){
        if (clients[i].connected){
            printf("stopping client, id: %d\n", i);
            client_queue = clients[i].queue_id;
            message_.mode = STOP;
            send_message(client_queue, &message_);
            receive_message(server_queue_id, &message_, STOP);
            printf("STOP received form client\n");
        }
    }
    delete_server(server_queue_id);
    printf("as you wanted, i've just stopped the server!\n");
}

void stop_sender(struct message* message_){
    printf("\nserver received STOP\n");
    message_->mode = STOP;
    int client_id = message_->sender_id;
    clients[client_id].connected = false;
}

void to_all(struct message* message_){
    int client = message_->sender_id;

    for (int i = 0; i < 10; i++){
        if (clients[i].id != 0 && clients[i].id != client){
            printf("sent message to: %d\n", clients[i].id);
            struct message new_message;
            sprintf(new_message.text, "%s", message_->text);
            new_message.sender_id = clients[i].id;
        }
    }
}

void init_client(key_t sender_id, pid_t pid){
    bool possible = false;
    for (int i=0; i<10; i++) {
        if (clients[i].connected == false){
            possible = true;
            break;
        }
        else {
            i++;
        }
    }
    if (possible == false){
        printf("cannot add new client, sory\n");
        return;
    }
    char *path = getenv("HOME");
    key_t client_key = ftok(path, pid);
    int queue_client = msgget(client_key, IPC_CREAT | 0666);
    struct message message_;
    message_.mode = INIT;
    message_.sender_id = current_id;
    current_id++;
}

void to_one(struct message* message_){
    int current_client = message_->sender_id;
    struct message new_message;
    sprintf(new_message.text, "%s", message_->text);
    new_message.sender = current_client;
    printf("message sent\n");
}

void list_clients(struct message* message_){
    printf("listing active clients:\n");
    for (int i = 0; i < 10; i++){
        if (clients[i].available){
            printf("id: %d, queue id: %d, status: available\n", clients[i].id, clients[i].queue_id);
        }
    }
}

// odbieranie wiadomości od klienta i ich obsługa
void handle_message(struct message* message_){
    switch (message_->mode) {
        case INIT:
            init_client(message_->sender_id, message_->queue_id);
            break;
        case STOP:
            stop_sender(message_);
            break;
        case TO_ALL:
            to_all(message_);
            break;
        case TO_ONE:
            to_one(message_);
            break;
        case LIST:
            list_clients(message_);
            break;
        default:
            printf("error with handling this message!\n");
            exit(1);
    }
}

int main(int args, char* argv[]){
    printf("hi, im server!\n");
    atexit(stop_server);
    // ustawia ze ta funkcja ma sie wykonac przed zakonczeniem programu (procesu)
    key_t key;
    // utworzenie unikalnego klucza kolejki
    if ((key = ftok(get_path(), ID)) == -1){
        printf("cannot create queue\n");
        return -1;
    }
    // utworzenie kolejki komunikatów
    if ((server_queue_id = msgget(key, IPC_CREAT | IPC_EXCL | 0666)) == -1){
        printf("cannot create queue\n");
        return -1;
    }
    printf("server queue id: %d\n", server_queue_id);
    // dodawanie klinetów (struktura klient u góry pliku)
    for (int i = 0; i < 10; i++){
        clients[i].available = true;
        clients[i].connected = false;
    }
    // clients added

    // odebranie sygnalu SIGINT
    signal(SIGINT, handle_SIGINT);

    struct message message_ex;
    while(1){
        // odebranie komunikatu z kolejki - msgrcv
        if (msgrcv(server_queue_id, &message_ex, sizeof(struct message) - sizeof(message_ex.mode), -INIT-1, 0) != -1){
            handle_message(&message_ex);
        }
        else {
            printf("error with received maessage\n");
            return -1;
        }
        handle_message(&message_ex);
    }
}
