#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct table *tablica = NULL;

int create_table(int size){
	if (size <= 0) {
		printf("Incorrect size!");
		return 1;
	}

    if (tablica != NULL) {
		printf("Table already exists!");
		return 1;
	}

    tablica = (struct table *) calloc(1, sizeof(struct table));
    tablica->size = size;
    tablica->arr = (struct entry**) calloc(size, sizeof(struct entry*));
    return 0;
}

int wc_files_to_temp_file(char** filenames, int tab_size){
    if (tab_size == 0) {
        return 1;
    }
    int command_size = 9;
    for (int i=0; i<tab_size; i++) {
        command_size += sizeof(filenames[i]) + 1;
    }
    char command[command_size];
    strcat(command, "wc ");
    for (int i=0; i<tab_size; i++) {
        strcat(command, filenames[i]);
        strcat(command, " ");
    }
    strcat(command, "> temp");
    system(command);
    command[0] = '\0';
    return 0;
}

int find_next_empty_block() {
    int table_size = tablica->size;
    for (int i=0; i<table_size; i++){
        if (tablica->arr[i] == NULL){
            return i;
        }
    }
    printf("No free blocks!");
    return -1;
}

int get_file_size(FILE* fp) {
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    rewind(fp);
    return size;
}

void create_block(int file_size, int i) {
    tablica->arr[i] = calloc(1, sizeof(struct entry));
    tablica->arr[i]->size = file_size;
    tablica->arr[i]->arr = calloc(file_size, sizeof(char));
}

int read_table_from_file(){
    FILE *fp;
    fp = fopen("temp", "r");
    if (!fp) {
        printf("Problem with opening this file!");
        return -1;
    }

    int file_size = get_file_size(fp);
    int i = find_next_empty_block();
    if (i == -1) {
        printf("Full table!");
        return -1;
    }

    fread(tablica->arr[i]->arr, 1, file_size, fp);
    create_block(file_size, i);
    fclose(fp);
    return 0;
}

int remove_block(int index){
    int table_size = tablica->size;
    if (index >= table_size || table_size < 0){
        printf("Index too big!");
        return -1;
    }
    if (tablica->arr[index] == NULL){
        printf("Lack of data!");
        return -1;
    }
    free(tablica->arr[index]->arr);
    free(tablica->arr[index]);
    return 0;
}
