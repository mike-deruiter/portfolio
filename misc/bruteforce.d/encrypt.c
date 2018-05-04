#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void encrypt(char *input);

int main(int argc, char *argv[])
{
    char *line = malloc(sizeof(char) * 80);
    size_t lb = 80;
    int len;

    len = getline(&line, &lb, stdin);
    
    // resize & chomp
    line = realloc(line, sizeof(char) * len - 1);
    line[len-1] = '\0';
    
    encrypt(line);
    
    printf("%s\n", line);
    
    exit(0);
}

void encrypt(char *input)
{
    srand(time(NULL));

    char key = rand() % 256;
    char *p;
    
    for (p = input; *p != '\0'; ++p) {
        *p ^= key;
    }
}
