#include "header.h"
#include <mqueue.h>

struct client clients[MAX_CLIENTS];
int queue_id1;

void send_message(struct message* msg, int q_id){
    if (mq_send(q_id, (char *) msg, sizeof(struct message), msg->type) == -1){
        printf("Error while sending message! %s\n", strerror(errno));
        exit(1);
    }
}

void handle_INIT(struct message *msg) {
    int slot = -1;
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(clients[i].connected == 0) {
            slot = i;
            clients[i].connected = 1;
            clients[i].id = i;
            strcpy(clients[i].queue_name, msg->text);
            clients[i].queue_id = mq_open(clients[i].queue_name, O_WRONLY);
            break;
        }
    }
    if(slot == -1) {
        printf("NO MORE SLOTS FOR CLIENTS\n");
        return;
    }

    printf("SERVER: Init client with id: %d\n", slot);
    struct message response = {.type = INIT, .sender_id = slot};

    send_message(&response, clients[slot].queue_id);
}

void stop_server(){
    printf("STOPPING SERVER...\n");
    struct message msg;
    msg.type = STOP;
    for (int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].connected) {
            send_message(&msg, clients[i].queue_id);

            mq_close(clients[i].queue_id);
        }
    }

    mq_close(queue_id1);
    mq_unlink(SERVER_Q);
    printf("\nSERVER STOPPED!\n");
    exit(0);
}

void handle_STOP(struct message* msg){
    clients[msg->sender_id].connected = 0;
    mq_close(clients[msg->sender_id].queue_id);
    clients[msg->sender_id].queue_id = -1;
}

void handle_TO_ONE(struct message* msg){
    if (clients[msg->to_connect_id].connected){
        send_message(msg, clients[msg->to_connect_id].queue_id);
    }
    printf("Message sent\n");
}

void handle_TO_ALL(struct message* msg){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].connected && i != msg->sender_id){
            send_message(msg, clients[i].queue_id);
        }
    }
    printf("Messages sent\n");
}

void handle_LIST(){
    printf("List of clients:\n");
    for(int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].connected){
            printf("Client with ID: %d\n", i);
        }
    }
}

void choose_mode(struct message *msg) {
    switch (msg->type) {
        case INIT:
            handle_INIT(msg);
            break;
        case LIST:
            handle_LIST();
            break;
        case TO_ALL:
            handle_TO_ALL(msg);
            break;
        case TO_ONE:
            handle_TO_ONE(msg);
            break;
        case STOP:
            handle_STOP(msg);
            break;
        default:
            printf("WRONG MESSAGE TYPE\n");
            break;
    }
}

void receive_message(int q_id, struct message* msg, unsigned int* type){
    if(mq_receive(q_id, (char *) msg, sizeof(struct message), type) == -1){
        printf("Error while receiving message! %s\n", strerror(errno));
        exit(1);
    }
}

int main() {

    for(int i = 0; i < MAX_CLIENTS; i++){
        clients[i].connected = 0;
    }

    struct mq_attr attr;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(struct message);
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;
    // tworzenie kolejki POSIX
    if((queue_id1 = mq_open(SERVER_Q,  O_RDWR | O_CREAT, 0666, &attr)) == -1){
        printf("mq_open");
        exit(1);
    }
    printf("queue_id1: %d\n", queue_id1);
    signal(SIGINT, stop_server);

    struct message msg;
    unsigned int type;

    while(1) {
        receive_message(queue_id1, &msg, &type);
        printf("Message recieved\n");
        fflush(stdout);
        choose_mode(&msg);
    }
}
