#include<stdio.h>
#include<stdlib.h>

char *input() {
    int size = 1024;
    int pos = 0;
    char *buffer = malloc(sizeof(char) * size);
    int c;
    if (!buffer) {
        fprintf(stderr, "lsh: allocation error");
        exit(EXIT_FAILURE);
    }
    printf(">>> ");
    while (1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            buffer[pos] = '\0';
            return buffer;
        } else {
            buffer[pos] = c;
        }
        pos++;
        if (pos > size) {
            size += 1024;
            buffer = realloc(buffer, size);
            if (!buffer) {
                fprintf(stderr, "lsh: allocation error");
                exit(EXIT_FAILURE);
            }
        }
    }
}


int main() {
    printf("%s", input());
    return 0;
}
