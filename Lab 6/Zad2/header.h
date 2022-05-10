#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>


#define SERVER_Q "/server_q1"
#define MAX_CLIENTS 10
#define MAX_MESSAGES 10
#define MAX_MESSAGE_LENGTH 256

#define STOP 1
#define LIST 2
#define TO_ALL 3
#define TO_ONE 4
#define INIT 5

struct message {
    long type;
    char text[MAX_MESSAGE_LENGTH];
    int sender_id;
    int to_connect_id;
};

struct client {
    int id;
    char queue_name[MAX_MESSAGE_LENGTH];
    int queue_id;
    int connected;
};

