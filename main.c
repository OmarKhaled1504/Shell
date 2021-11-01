#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <wait.h>
#include <string.h>

char *input() {
    int size = 1024, pos = 0;

    char *buffer = malloc(sizeof(char) * size);
    int c;
    if (!buffer) {
        fprintf(stderr, "lsh: allocation error");
        exit(EXIT_FAILURE);
    }
    printf(">>> ");
    while (1) {
        c = getchar(); //read a character
        if (c == EOF || c == '\n') { //if we hit EOF replace with NULL char and return
            buffer[pos] = '\0';
            return buffer;
        } else {
            buffer[pos] = c;
        }
        pos++;
        if (pos > size) { // if buffer is exceeded reallocate memory
            size += 1024;
            buffer = realloc(buffer, size);
            if (!buffer) {
                fprintf(stderr, "lsh: allocation error");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **parse(char *line, int *waitflag) {
    int size = 64, pos = 0;
    char *delim = " \t\r\n\a"; //delimiters be any white space
    char **tokens = malloc(size * sizeof(char *));
    char *token;
    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, delim); //split the input into arguments
    while (token != NULL) {
        tokens[pos++] = token;
        if (pos >= size) { //if buffer is exceeded reallocate
            size += 64;
            tokens = realloc(tokens, size * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, delim);
    }
    if (strcmp(tokens[pos - 1], "&") == 0) {
        *waitflag = 0;
        tokens[pos - 1] = NULL; //add null in place of &
    }
    tokens[pos] = NULL; //add null to the end of the string
    return tokens;
}

void exec(char **args, int *waitflag) {
    int pid = fork();
    if (pid == -1) {
        return;
    }
    if (pid == 0)
        execvp(args[0], args);
    else {
        if (waitflag == 1)
            wait(NULL);
    }
}

void run() {
    int *waitflag = 1;
    while (1) {
        char *command = input();
        if (strcmp(command, "exit") != 0) {
            char **args = parse(command, &waitflag);
            exec(args, &waitflag);
        } else break;
    }
}

int main(int argc, char *argv[]) {
    run();
    return 0;
}
