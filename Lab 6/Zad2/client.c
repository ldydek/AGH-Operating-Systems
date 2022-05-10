#include "header.h"

int server_queue_id = 0;
int client_queue_id = 0;
int id = 0;

char *get_name(){
    char* name = calloc(MAX_MESSAGE_LENGTH, sizeof(char));
    sprintf(name, "/client_q_%d", getpid());
    return name;
}

void receive_message(int q_id, struct message* msg, unsigned int* type){
    if(mq_receive(q_id, (char *) msg, sizeof(struct message), type) == -1){
        printf("Error while receiving message! %s\n", strerror(errno));
        exit(1);
    }
}

void send_message(struct message* msg, int q_id){
    if (mq_send(q_id, (char *) msg, sizeof(struct message), msg->type) == -1){
        printf("Error while sending message! %s\n", strerror(errno));
        exit(1);
    }
}

void send_INIT(){
    struct message send_init;
    send_init.type = INIT;
    strcpy(send_init.text, get_name());
    send_message(&send_init, server_queue_id);
}

void send_LIST(){
    struct message send_list;
    send_list.type = LIST;
    send_message(&send_list, server_queue_id);
}

void send_STOP(){
    struct message send_stop;
    send_stop.sender_id = id;
    send_stop.type = STOP;
    send_message(&send_stop, server_queue_id);
    msgctl(client_queue_id, IPC_RMID, NULL);
    exit(0);
}

void send_TO_ALL(char *text){
    struct message send_to_all;
    send_to_all.sender_id = id;
    send_to_all.type = TO_ALL;
    strcpy(send_to_all.text, text);
    send_message(&send_to_all, server_queue_id);
}

void send_TO_ONE(char* text, int send_to){
    struct message send_to_one;
    send_to_one.sender_id = id;
    send_to_one.type = TO_ONE;
    send_to_one.to_connect_id = send_to;
    strcpy(send_to_one.text, text);
    send_message(&send_to_one, server_queue_id);
}

void handle_INIT(struct message* msg){
    id = msg->sender_id;
}

void handle_text(struct message* msg){
    printf("New message from %d\n", msg->sender_id);
    printf("%s\n", msg->text);
}

void handle_STOP(){
    mq_close(client_queue_id);
    mq_close(server_queue_id);
    mq_unlink(get_name());
    exit(0);
}

int is_empty(int q){
    struct mq_attr attr;
    mq_getattr(q, &attr);
    if (attr.mq_curmsgs == 0) return 1;
    return 0;
}

void catcher(){
    while (is_empty(client_queue_id) == 0){
        struct message message1;
        unsigned int type;
        receive_message(client_queue_id, &message1, &type);

        switch (message1.type)
        {
            case STOP:
                handle_STOP();
                break;
            case INIT:
                handle_INIT(&message1);
                break;
            case TO_ALL:
                handle_text(&message1);
                break;
            case TO_ONE:
                handle_text(&message1);
                break;
            default:
                break;
        }
    }
}


int check_input(){
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return (FD_ISSET(0, &fds));
}

int main(){
    signal(SIGINT, send_STOP);
    server_queue_id = mq_open(SERVER_Q, O_WRONLY);
    struct mq_attr attr = {.mq_maxmsg = MAX_MESSAGES, .mq_msgsize = sizeof(struct message), .mq_flags = 0, .mq_curmsgs = 0};
    if((client_queue_id = mq_open(get_name(),  O_RDWR | O_CREAT | O_NONBLOCK, 0666, &attr)) == -1){
        printf("mq_open");
        exit(1);
    }

    printf("client_qid: %d\n", client_queue_id);

    send_INIT();

    char buffer[MAX_MESSAGE_LENGTH];

    while(1){
        if (check_input()) {
            fgets(buffer, MAX_MESSAGE_LENGTH, stdin);

            if (strcmp("LIST\n", buffer) == 0) {
                send_LIST();
            } else if (strcmp("STOP\n", buffer) == 0) {
                send_STOP();
            } else if (strcmp("2ONE\n", buffer) == 0) {
                printf("Enter client ID: ");
                fgets(buffer, MAX_MESSAGE_LENGTH, stdin);
                int id_to_sent = atoi(buffer);
                printf("Enter message text: ");
                fgets(buffer, MAX_MESSAGE_LENGTH, stdin);
                send_TO_ONE(buffer, id_to_sent);
            } else if (strcmp("2ALL\n", buffer) == 0) {
                printf("Enter message text: ");
                fgets(buffer, MAX_MESSAGE_LENGTH, stdin);
                send_TO_ALL(buffer);
            }
            else {
                printf("Unknown command: %s\n", buffer);
            }
        }

        sleep(1);
        catcher();
    }
}
