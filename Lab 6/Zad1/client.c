#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include "header.h"

#define ID 'S'
#define CLIENT_ID getpid()
#define BUFFER_SIZE 256

int id;


// wysylanie komunikatu do kolejki serwera
void send_message(int queue_id, struct message *message_){
    if(msgsnd(queue_id, message_, sizeof(struct message) - sizeof(long), 0) == -1){
        printf("cannot send this message\n");
        exit(1);
    }
}

char *get_path(){
    char *path = getenv("HOME");
    if (path == NULL){
        path = getpwuid(getuid()) -> pw_dir;
    }
    return path;
}

void *delete_client(int queue){
    msgctl(queue, IPC_RMID, NULL);
}

// odbieranie wiadomoœci od kolejki serwerwa systemu V
void receive_message(int queue_id, struct message* message_, long type){
    if(msgrcv(queue_id, message_, sizeof(struct message) - sizeof(long), message_->mode, 0) == -1){
        printf("cannot receive message\n");
        exit(1);
    }
}

void send_STOP(int server_queue, int client_queue){
    printf("client sent STOP\n");
    struct message message_;
    send_message(server_queue, &message_);
    msgctl(client_queue, IPC_RMID, NULL);
    exit(0);
}

bool check(){
    struct timeval timeval_;
    fd_set fds;
    timeval_.tv_sec = 1;
    timeval_.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeval_);
    return FD_ISSET(0, &fds);
}

void to_one(int current_id){
    char buffer0[BUFFER_SIZE];
    char buffer1[BUFFER_SIZE];
    printf("Give me message:\n");
    scanf("%c", buffer0);
    printf("Give me user id:\n");
    scanf("%c", buffer1);
    int id = atoi(buffer1);
    struct message sent_message;
    sent_message.mode = TO_ONE;
    sent_message.sender_id = current_id;
    sent_message.queue_id = id;
    strcpy(sent_message.text, buffer0);
    send_message(current_id, &sent_message);
    printf("Message was sent\n");
}

bool is_empty(int queue){
    struct msqid_ds buffer;
    msgctl(queue, IPC_STAT, &buffer);
    if (buffer.msg_qnum != 0) {
        return false;
    }
    return true;
}

// wypisanie listy wszystkich aktywnych klientów
void send_list(int server_queue){
    printf("client sent list\n");
    struct message message_ = {.mode = LIST};
    send_message(server_queue, &message_);
}

void to_all(int id){
    struct message sent_message;
    char buffer[BUFFER_SIZE];
    sent_message.mode = TO_ALL;
    sent_message.queue_id = id;
    printf("give me message to pass:\n");
    scanf("%c", buffer);
    strcpy(sent_message.text, buffer);
    send_message(id, &sent_message);
    printf("send all messages!\n");
}

int main (){
    int server_queue = 0;
    int client_queue = 0;
    printf("Hi, here client!\n");
    signal(SIGINT, send_STOP);
    atexit(delete_client(client_queue));
    key_t server_key;
    // utworzenie klucza do kolejki
    if ((server_key = ftok(get_path(), ID)) == -1){
        printf("Cannot generate key!\n");
        return -1;
    }
    // utworzenie kolejki serwera
    if ((server_queue = msgget(server_key, IPC_CREAT)) == -1){
        printf("Cannot create server queue!\n");
        return -1;
    }
    printf("Server queue: %d\n", server_queue);
    key_t client_key;
    if ((client_key = ftok(get_path(), CLIENT_ID)) == -1){
        printf("Cannot create client key!\n");
        return -1;
    }
    // utworzenie kolejki klienta
    if ((client_queue = msgget(client_key, IPC_CREAT | IPC_EXCL | 0666)) == -1){
        printf("cannot create client queue\n");
        return -1;
    }
    printf("client queue id: %d\n", client_queue);

    // komunikat init klienta do serwera
    struct message sent_message;
    sent_message.queue_id = client_queue;
    sent_message.mode = INIT;
    send_message(server_queue, &sent_message);
    struct message received_message;
    receive_message(client_queue, &received_message, INIT);
    printf("Client id is: \n%d\n\n", received_message.sender_id);
    id = received_message.sender_id;
    // id - identyfikator klienta

    // zlecenia klienta do serwera
    char buffer[BUFFER_SIZE];
    while(true){
        if (check()){
            fgets(buffer, 256, stdin);
            if (strcmp("LIST\n", buffer) == 0){
                send_list(server_queue);
            }
            else if(strcmp("STOP\n", buffer) == 0){
                send_STOP(server_queue, client_queue);
                exit(0);
            }
            else if(strcmp("2ALL\n", buffer) == 0){
                to_all(client_queue);
            }
            else if(strcmp("2ONE\n", buffer) == 0){
                to_one(client_queue);
            }
            else {
                printf("i got the message\n");
            }
        }
        sleep(1);
    }
}
