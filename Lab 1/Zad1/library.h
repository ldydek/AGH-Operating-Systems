#ifndef SYSOPY_LIB_ZAD1_H
#define SYSOPY_LIB_ZAD1_H

struct entry{
    int size;
    char* arr;
};

struct table{
    int size;
    struct entry** arr;
};


int create_table(int size);
int wc_files_to_temp_file(char** filenames, int len);
int read_table_from_file();
int remove_block(int index);

#endif
