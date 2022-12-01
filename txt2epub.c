#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_STR_SIZE

int main(int argc, char *argv[] ){

    char* zip[argc+4];

    pid_t pid;
    for(int j=0 ; j < argc - 1;j++){

        char* org_file = (char*)malloc(MAX_STR_SIZE sizeof(char));
        strcpy(org_file,argv[j+1]);
        int org_file_len = strlen(org_file);
        org_file[org_file_len-4] = '\0';
        char* file_epub = (char*)malloc(MAX_STR_SIZE sizeof(char));
        strcpy (file_epub,org_file);
        strcat(file_epub,".epub");
        char* parametros[] = {"pandoc",argv[j+1], "-o", file_epub, NULL};

        if ((pid = fork()) == -1){
            perror("fork");
            return EXIT_FAILURE;
        }
        else if (pid == 0){
            printf("%s%d%s%s%s%s\n","[pid",getpid(),"]"," converting ",argv[j+1]," ...");
            execvp("pandoc", parametros);
            exit(1);
        }
        else{

            if (waitpid(pid, NULL, 0) == -1){
                perror("waitpid");
                return EXIT_FAILURE;
            }
            zip[j+2] = malloc((strlen(file_epub)+1) * sizeof(char));
            strcpy(zip[j+2], file_epub);

        }
    }
    zip[0] = "zip";
    zip[1] = "ebooks.zip";
    zip[argc+2] = "--quiet";
    zip[argc+3] = NULL;
    execvp(zip[0], zip);
}