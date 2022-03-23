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


struct table* create_table(struct table *tablica, int size);
int wc_files_to_temp_file(char** filenames, int len);
int read_data_from_file(struct table* tablica);
void create_block(struct table *tablica, int file_size, int i);
int remove_block(struct table* tablica, int index);
int remove_table(struct table* tablica);
int find_next_empty_block(struct table* tablica);

#endif
