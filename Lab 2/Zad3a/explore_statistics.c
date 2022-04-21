#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "explore_statistics"
#include <string.h>

int files;
int directories;
int special_sign_files;
int special_block_files;
int fifo_queues;
int links;
int sockets;

void show_stats(char path[], struct stat *file_stats) {
    char buffer[256] = "unknown";
    if (S_ISREG(file_stats->st_mode)) {
        strcpy(file_type, "file");
        files++;
    }
    else if (S_ISDIR(file_stats->st_mode)) {
        strcpy(file_type, "dir");
        directories++;
    }
    else if (S_ISCHR(file_stats->st_mode)) {
        strcpy(file_type, "char dev");
        special_sign_files++;
    }
    else if (S_ISBLK(file_stats->st_mode)) {
        strcpy(file_type, "block dev");
        special_block_files++;
    }
    else if (S_ISFIFO(file_stats->st_mode)) {
        strcpy(file_type, "fifo");
        fifo_queues++;
    }
    else if (S_ISLNK(file_stats->st_mode)) {
        strcpy(file_type, "slink");
        links++;
    }
    else if (S_ISSOCK(file_stats->st_mode)) {
        strcpy(file_type, "socket");
        sockets++;
    }
    printf("Path: %s, links number: %ld, type: %s, size: %ld, last access time: %s, last modification time: %s\n",
           path, file_stats->st_nlink, file_type, file_stats->st_size, access_str, modif_str);
}


void traverse_the_tree(char *path, int root) {
    DIR *dir = opendir(path);
    if (dir == NULL){
        perror("Problem with opening this directory! \n");
        exit(10);
    }
    struct dirent *file;
    struct stat file_stats;
    if (lstat(path, &file_stats) == -1) {
        printf("Problems with writing statistics to the buffer!");
        exit(10);
    }
    show_stats(path, &file_stats);

    while (entry = readdir(dir) != NULL) {
        char next_path[256];
        strcpy(next_path, path);
        strcat(next_path, "/");
        strcat(next_path, entry->d_name);

         if (lstat(next_path, &file_stats) < 0){
            perror("Problem with lstating file");
            exit(10);
        }

        if (S_ISDIR(file_stats.st_mode)){
            if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0){
                continue;
            }
            traverse_the_tree(next_path, 0);
        }
    }
    closedir(dir);
    if(root == 1)
        printf("Regular files: %d, Directories: %d, Character devices: %d, Block devices: %d, FIFOs: %d, Slinks: %d, Sockets: %d \n\n\n",
           files, directories, special_sign_files, special_block_files, fifo_queues, links, sockets);
}
