#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wywo³anie %s œcie¿ka", argv[0]);
        return 1;
    }
    struct dirent *pozycja;

    DIR *pDir = opendir(argv[1]);
/*Otwórz katalog, w przypadku b³êdu otwarcia zwróæ b³¹d funkcji otwieraj¹cej i zwrócæ 1.
Wyœwietl zawartoœæ katalogu katalog, pomijaj¹c "." i ".."
Jeœli podczas tej operacji wartoœæ errno zostanie ustawiona, zwróæ b³¹d funkcji czytaj¹cej oraz wartoœæ 2. */
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
