#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

char *input() {                                             //read input command
    int size = 1024, pos = 0;
    char *buffer = malloc(sizeof(char) * size);
    int c;
    if (!buffer) {
        fprintf(stderr, "lsh: allocation error");
        exit(EXIT_FAILURE);
    }
    printf(">>> ");
    while (1) {
        c = getchar();                                      // read a character
        if (c == EOF || c == '\n') {                        // if we hit EOF replace with NULL char and return
            buffer[pos] = '\0';
            return buffer;
        } else {
            buffer[pos] = c;
        }
        pos++;
        if (pos > size) {                                   // if buffer is exceeded reallocate memory
            size += 1024;
            buffer = realloc(buffer, size);
            if (!buffer) {
                fprintf(stderr, "lsh: allocation error");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **parse(char *line, int *waitflag) {                   // parse input command into arguments
    int size = 64, pos = 0;
    char *delim = " \t\r\n\a";                              // delimiters be any white space
    char **tokens = malloc(size * sizeof(char *));
    char *token;
    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, delim);                            // split the input into arguments
    while (token != NULL) {
        tokens[pos++] = token;
        if (pos >= size) {                                  // if buffer is exceeded reallocate
            size += 64;
            tokens = realloc(tokens, size * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, delim);
    }
    if (strcmp(tokens[pos - 1], "&") == 0) {                // check for "&" in the end and set wait flag accordingly
        *waitflag = 0;
        tokens[pos - 1] = NULL;                             // add null in place of &
    } else
        *waitflag = 1;                                      // resets waitflag for future commands
    tokens[pos] = NULL;                                     // add null to the end of the string
    return tokens;
}

void handler() {                                            // SIGCHLD handler to print if a child process was terminated in a log file
    FILE *fp = fopen("/home/ketch/CLionProjects/Shell/log.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!");
        return;
    }
    fprintf(fp, "Child process was terminated\n");
    fclose(fp);
}

void exec(char **args, int *waitflag) {                     // creating a child process then executing command args
    pid_t pid;                                              // then executing the command in it
    signal(SIGCHLD, handler);                               // call to SIGCHLD handler
    pid = fork();
    if (pid == -1) {
        return;
    }
    if (pid == 0) {
        if (strcmp(args[0], "cd") == 0) { chdir(args[1]); } // handling "cd" command
        else {
            if (execvp(args[0], args) == -1)
                printf("Error occurred while executing command.\n");
        }
    } else {
        if (*waitflag ==
            1)                                 // if "&" is not found in the command args the parent waits for the child
            waitpid(pid, NULL, 0);                          // waits for the specific parent that made the child
    }                                                       // so the parent doesnt resume when a random child terminates
}

void run() {
    int *waitflag = 1;
    while (1) {                                             // execution loop
        char *command = input();
        if (strcmp(command, "exit") != 0) {                 // implementing "exit" command
            char **args = parse(command, &waitflag);
            exec(args, &waitflag);
        } else break;
    }
}

int main() {
    run();                                                  // call to execution loop
    return 0;
}
