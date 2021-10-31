#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
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

char **parse(char *line) {
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
    tokens[pos] = NULL; //add null to the end of the string
    return tokens;
}

int main(int argc, char *argv[]) {
    char *command = input();
    char **args = parse(command);
    int pid = fork();
    if (pid == -1) {
        return 1;
    }
    if (pid == 0)
        execvp(args[0], args);
    else {
        wait(NULL);
        printf("success!");
    }
    return 0;
}
