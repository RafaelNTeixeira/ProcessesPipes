#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char* argv[]){
    if (argc != 4){
        printf("%s","usage: samples file numberfrags maxfragsize\n");
        return EXIT_FAILURE;    
    }

    FILE* fp;
    fp = fopen(argv[1],"r");
    if (!fp){
        perror("file doesn't exist/file doesn't open\n");
    }
    
    int n = atoi(argv[2]);
    int m = atoi(argv[3]);
    fseek(fp, 0, SEEK_END);
    int upper = ftell(fp);

    srand(0);
    for (int i = 0; i < n; i++) {
        int random = rand() % (upper + 1- m);
        fseek(fp, random, SEEK_SET);

        printf("%c", '>');
        for (int i = 0; i < m; i++) {
            char ch = fgetc(fp);
            if (ch < 32) {
                ch = ' ';
            }
            printf("%c", ch);
        }
        printf("%c\n", '<');
    }
    fclose(fp);
}