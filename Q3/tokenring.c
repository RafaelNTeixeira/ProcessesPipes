#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define LINE_SIZE 256

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);
    float p = atof(argv[2]);
    int t = atoi(argv[3]);
    char* pipes[n];
    int fd[2];

    for (int i = 0; i<n; i++) {
        char* pipe = (char*)malloc(sizeof(char) * LINE_SIZE);
        if (i+1 == n) {
            sprintf(pipe, "%s%d%s%d", "pipe", i+1, "to", 1);
        }
        else {
            sprintf(pipe, "%s%d%s%d", "pipe", i+1, "to", i+2);
        }
        pipes[i] = pipe;
        mkfifo(pipe, 0666);
    }

    pid_t pid;

    int i;     
    for (i = 0; i < n; i++) {
        if ((pid = fork()) == -1) {
            perror("Error");
            return EXIT_FAILURE;
        }

        else if (pid == 0) { 
            break;
        }
    }

    if (pid == 0) {
        srand(time(NULL)% getpid());
        int current_token = 0; 
        int last_token = -1;
        char* reader;
        char* writer = pipes[i];
        if (i >= n-1) {
            reader = pipes[0];
        }
        else{    
            reader = pipes[i+1];
        }

        if (i == 0) {
            fd[1] = open(writer, O_WRONLY);
            fd[0] = open(reader, O_RDONLY);
        }

        else {
            fd[0] = open(reader, O_RDONLY);
            fd[1] = open(writer, O_WRONLY);
        }
        
        if (i == 0) {
            write(fd[1], &current_token, sizeof(int));
        }

        while(1) {
            read(fd[0], &current_token, sizeof(int));
            if (last_token != current_token) {
                if ( (rand()) / ((float) RAND_MAX ) <= p) {
                    printf("[p%d] lock token (value = %d)\n", i + 1, current_token);
                    sleep(t);
                    printf("[p%d] unlock token\n", i + 1);
                }
                last_token = current_token;
                current_token += 1;
                write(fd[1], &current_token, sizeof(int));
            }
        }
        return EXIT_SUCCESS;
    }
    return EXIT_SUCCESS;
}