#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

#define IMAGE_SIZE 256

int **image = NULL;
int **negative_image = NULL;
int w = 0;
int h = 0;
int brightness = 0;
int threads_number = 0;

void load_image(char* filename){
    FILE *file = fopen(filename, "r");
    char *buffer = calloc(IMAGE_SIZE, sizeof(char));

    // przeszukiwanie pliku, pomijając autora i nagłówki
    fgets(buffer, IMAGE_SIZE, file);
    fgets(buffer, IMAGE_SIZE, file);
    fgets(buffer, IMAGE_SIZE, file);
    sscanf(buffer, "%d %d\n", &w, &h);
    fgets(buffer, IMAGE_SIZE, file);
    sscanf(buffer, "%d \n", &brightness);

    image = (int **) calloc(h, sizeof(int*));
    for (int i = 0; i < h; i++)
        image[i] = (int *) calloc(w, sizeof(int));

    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            fscanf(file, "%d", &image[i][j]);
        }
    }
    fclose(file);
}

void *numbers_method(void* arg){
    int id = *((int *) arg);
    struct timeval stop, start;

    int start_color = (brightness + 1) / threads_number * id;
    int end_color = ((brightness + 1) / threads_number * (id + 1));
    if (id == threads_number - 1) end_color = brightness + 1;

    gettimeofday(&start, NULL);
    // zaczynamy mierzyć czas

    for (int i=0; i<h; i++){
        for (int j=0; j<w; j++){
            if (start_color <= image[i][j] && image[i][j] < end_color)
                negative_image[i][j] = brightness - image[i][j];
        }
    }

    gettimeofday(&stop, NULL);
    // kończymy mierzyć czas

    long unsigned int time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    pthread_exit(NULL);
}

void *block_method(void* arg) {
    int id = *((int *) arg);
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    // zaczynamy mierzyć czas

    int start1 = (id) * ceil(w / threads_number);
    int end = ((id + 1) * ceil(w / threads_number) - 1);
    if (id == threads_number - 1) {
        end = w+1;
    }

    for (int i = 0; i < h; i++){
        for (int j = start1; j <= end; j++)
            negative_image[i][j] = brightness - image[i][j];
    }

    gettimeofday(&stop, NULL);
    // kończymy mierzyć czas

    long unsigned int time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    pthread_exit(NULL);
}

//tworzenie nowych wątków w zależności od wariantu
pthread_t* start_threads(char* mode) {
    pthread_t *threads = calloc(threads_number, sizeof(pthread_t));
    int *threads_ids = calloc(threads_number, sizeof(int));

    for(int i=0; i<threads_number; i++){
        threads_ids[i] = i;
        if (strcmp(mode, "numbers") == 0)
            pthread_create(&threads[i], NULL, &numbers_method, &threads_ids[i]);
        else
            pthread_create(&threads[i], NULL, &block_method, &threads_ids[i]);
    }
    return threads;
}

void write_threads_results(pthread_t* threads, FILE* times_file) {
    long unsigned int time;
    for(int i = 0; i < threads_number; i++) {
        pthread_join(threads[i], (void **) time);
        printf("thread id: %3d - time: %lu μs\n", i, time);
        fprintf(times_file, "thread id: %3d - time: %lu μs\n", i, time);
    }
}

void write_total_time(struct timeval start, struct timeval stop, FILE* times_file) {
    long unsigned int time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("Total time: %lu μs\n", time);
    fprintf(times_file, "Total time: %lu μs\n\n\n", time);
}

void save_negative_image(char* filename) {
    FILE *f = fopen(filename, "w");

    fprintf(f, "P2\n");
    fprintf(f, "%d %d\n", w, h);
    fprintf(f, "%d\n", brightness);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++)
            fprintf(f, "%d ", negative_image[i][j]);
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(int argc, char **argv){
    if (argc != 5){
        printf("Wrong arguments number!!!\n");
        return 0;
    }
    // czytanie danych wejściowych
    threads_number = atoi(argv[1]);
    char *mode = argv[2];
    char *input_file = argv[3];
    char *output_file = argv[4];
    struct timeval start, stop;

    // otwieranie pliku pgm i przepisywanie wartości z pierwotnego
    load_image(input_file);
    negative_image = calloc(h, sizeof(int *));
    for (int i = 0; i < h; i++) {
        negative_image[i] = calloc(w, sizeof(int));
    }

    // otwieranie pliku z czasami i zapisanie nagłówka
    FILE* times_file = fopen("times.txt", "a");

    printf("Number of threads: %d\n", threads_number);
    printf("Method used: %s\n", mode);
    fprintf(times_file, "Number of threads: %d\n", threads_number);
    fprintf(times_file, "Method used: %s\n", mode);

    gettimeofday(&start, NULL);

    pthread_t* threads = start_threads(mode);
    write_threads_results(threads, times_file);
    // zapisywanie do pliku resultatów

    gettimeofday(&stop, NULL);

    // total time
    long unsigned int time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("Total time: %lu μs\n", time);
    fprintf(times_file, "Total time: %lu μs\n\n\n", time);

    fclose(times_file);

    save_negative_image(output_file);
    for (int i = 0; i < h; i++) {
        free(negative_image[i]);
    }
    free(negative_image);
    free(threads);
    return 0;
}
