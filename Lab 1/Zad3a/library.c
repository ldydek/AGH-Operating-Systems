#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct table* create_table(struct table *tablica, int size){
	if (size <= 0) {
		printf("Incorrect size!\n");
		return NULL;
	}

    if (tablica != NULL) {
		printf("Table already exists!\n");
        return NULL;
	}

    tablica = calloc(1, sizeof(struct table));
    tablica->size = size;
    tablica->arr = calloc(size, sizeof(struct entry*));
    return tablica;
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
    strcpy(command, "wc ");
    for (int i=0; i<tab_size; i++) {
        strcat(command, filenames[i]);
        strcat(command, " ");
    }
    strcat(command, ">> temp.txt");
    system(command);
    return 0;
}

int find_next_empty_block(struct table* tablica) {
    int table_size = tablica->size;
    for (int i=0; i<table_size; i++){
        if (tablica->arr[i] == NULL){
            return i;
        }
    }
    printf("No free blocks!\n");
    return -1;
}

long get_file_size(FILE* fp) {
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return size;
}

void create_block(struct table *tablica, int file_size, int i) {
    tablica->arr[i] = calloc(1, sizeof(struct entry));
    tablica->arr[i]->size = file_size;
    tablica->arr[i]->arr = calloc(file_size, sizeof(char));
}

int read_data_from_file(struct table* tablica){
    if (tablica == NULL) {
        printf("Table doesn't exist!\n");
        return -1;
    }
    FILE *fp;
    fp = fopen("temp.txt", "r");
    if (!fp) {
        printf("Problem with opening this file!\n");
        return -1;
    }

    int file_size = get_file_size(fp);
    int i = find_next_empty_block(tablica);
    if (i == -1) {
        printf("Full table!\n");
        return -1;
    }

    create_block(tablica, file_size+1, i);
    fread(tablica->arr[i]->arr, 1, file_size+1, fp);
    fclose(fp);
    system("rm temp.txt");
    return i;
}

int remove_block(struct table* tablica, int index){
    int table_size = tablica->size;
    if (index >= table_size || index < 0){
        printf("Wrong index!\n");
        return -1;
    }
    if (tablica->arr[index] == NULL){
        printf("Lack of data!\n");
        return -1;
    }
    free(tablica->arr[index]->arr);
    free(tablica->arr[index]);
    tablica->arr[index] = NULL;
    return index;
}

int remove_table(struct table* tablica) {
    if (tablica == NULL) {
        printf("Table already doesn't exist\n");
        return 0;
    }
    int table_size = tablica->size;
    for (int i=0; i<table_size; i++) {
        if (tablica->arr[i] != NULL) {
            remove_block(tablica, i);
        }
    }
    free(tablica->arr);
    free(tablica);
    return 0;
}
