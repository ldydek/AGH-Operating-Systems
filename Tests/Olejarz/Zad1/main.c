#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wywo�anie %s �cie�ka", argv[0]);
        return 1;
    }
    struct dirent *pozycja;

    DIR *pDir = opendir(argv[1]);
/*Otw�rz katalog, w przypadku b��du otwarcia zwr�� b��d funkcji otwieraj�cej i zwr�c� 1.
Wy�wietl zawarto�� katalogu katalog, pomijaj�c "." i ".."
Je�li podczas tej operacji warto�� errno zostanie ustawiona, zwr�� b��d funkcji czytaj�cej oraz warto�� 2. */
    if (pDir == NULL) {
        return 1;
    }

    char filename[255];
    while ((pozycja = readdir(pDir)) != NULL) {
        if (strcmp(pozycja->d_name, ".") == 0 || strcmp(pozycja->d_name, "..") == 0){
            continue;
        }
        strcpy(filename, pozycja->d_name);
        printf("%s\n", filename);
    }
    closedir(pDir);
    return 0;
    // DONE
}
