#include <time.h>

#define PROJECT_ID  2341
#define CLIENT_PROJ_ID getpid()
#define STOP 1
#define LIST 2
#define TO_ALL 3
#define TO_ONE 4
#define INIT 5
#define MAX_CLIENTS 4
#define MAX_MESSAGE 256
#define BUFFER_SIZE 256

struct message {
    long mode;
    char text[BUFFER_SIZE];
    pid_t sender;
    int queue_id;
    int sender_id;
};

struct client {
    int id;
    int queue_id;
    int connection_id;
    bool connected;
    bool available;
};
