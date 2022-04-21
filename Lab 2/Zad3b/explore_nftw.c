#include <ftw.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include "explore_nftw.h"


int reg_file_num;
int dir_num;
int char_dev_num;
int block_dev_num;
int fifo_num;
int slink_num;
int socket_num;

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

    struct tm modif_tm;
    localtime_r(&ptr_to_stat->st_mtime, &modif_tm);
    char modif_str[MAX_TIME_LEN];
    strftime(modif_str, MAX_TIME_LEN, time_format, &modif_tm);

    struct tm access_tm;
    localtime_r(&ptr_to_stat->st_atime, &access_tm);
    char access_str[MAX_TIME_LEN];
    strftime(access_str, MAX_TIME_LEN, time_format, &access_tm);

    printf("Path: %s, links number: %ld, type: %s, size: %ld, last access time: %s, last modification time: %s\n",
           path, file_stats->st_nlink, file_type, file_stats->st_size, access_str, modif_str);
}

int nftw_print_function(const char *filename, const struct stat *statptr, int fileflags, struct FTW *pfwt){
    print_from_stat(filename, statptr);
    return 0;
}

void traverse_nftc(char *path){
    if (path == NULL) return;
    int flags = FTW_PHYS;
    nftw(path, nftw_print_function, MAX_FD_NUM, flags);
    printf("Regular files: %d, Directories: %d, Character devices: %d, Block devices: %d, FIFOs: %d, Slinks: %d, Sockets: %d \n\n\n",
           reg_file_num, dir_num, char_dev_num, block_dev_num, fifo_num, slink_num, socket_num);
}
