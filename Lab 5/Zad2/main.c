#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int send_mail(char *mail, char *topic, char *content) {
    int size = sizeof(mail) + sizeof(topic) + sizeof(content) + sizeof(char)*3;
    char text[size];
    for (int i=0; i<size; i++) {
        text[i] = '\0';
    }
    sprintf(text, "%s\t%s\t%s\n", mail, topic, content);
    char *command = "cat >> mails.txt";
    FILE* cat_input = popen(command, "w");
    fputs(text, cat_input);
    pclose(cat_input);
    return 0;
}

int sort_mails(char parameter[]){
    char* command;
    if (strcmp(parameter,"sender") == 0){
        command = "cat mails.txt | sort -k1";
    }
    else if (strcmp(parameter,"date") == 0){
        command = "cat mails.txt | sort -M";
    }

    FILE* file_with_mails = popen(command, "r");
    char c;
    while (fread(&c, sizeof(char), 1, file_with_mails) > 0) {
        printf("%c", c);
    }
    pclose(file_with_mails);
    return 0;
}

int main(int argc, char **argv) {
    if ((argc == 2) || (argc == 4)) {
        if(argc == 2){
            sort_mails(argv[1]);
        }
        if(argc == 4){
            send_mail(argv[1], argv[2], argv[3]);
        }
    }
    else {
        printf("One or three parameters required!");
    }
    return 0;
}
