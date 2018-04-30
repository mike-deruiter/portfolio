/* bruteforce - encrypt input w/ single-byte XOR & then decrypt it w/ brute
   force. 
   
   TODO: Make case-insensitive
   TODO: Split into 2 programs                                              */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define HASH_SIZE   20
#define LINE_BUFFER 50

void encrypt(char *input);
int bruteforce(char *input, int len);
bool string_decrypts(char *input, int len);

void read_process_words(FILE *input, void do_work(char *t));

// hash table functions
void init_hash();
unsigned long convert_to_key(char *s);
bool hash_contains(char *s);
void hash_insert(char *s);

typedef struct node {
    long key;
    struct node *next;
} hash_node;

hash_node *hash_array[HASH_SIZE];

int main(int argc, char *argv[])
{
    int key;

    char *line = malloc(sizeof(char) * 80);
    size_t lb = 80;
    int len;
    
    init_hash();
    
    fprintf(stderr, "Loading dictionary...\n");
    FILE *dict = fopen("/usr/share/dict/words", "r");
    read_process_words(dict, hash_insert);
    fclose(dict);
    fprintf(stderr, "Done. Enter message\n");
    
    // change these next 2 lines  
    len = getline(&line, &lb, stdin);
    // resize & chomp
    --len;
    line = realloc(line, sizeof(char) * len);
    line[len] = '\0';
    
    encrypt(line);
    fprintf(stderr, "\n");
    fprintf(stderr, "Encrypted Message: %s\n", line);
    
    if ((key = bruteforce(line, len)) != -1) {
        fprintf(stderr, "Key Found: %d\nDecrypted Message: %s\n", key, line);
    } else {
        fprintf(stderr, "%s: cannot decode message\n", argv[0]);
        exit(1);
    }
    
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

int bruteforce(char *orig, int len)
{
    int i;
    char *p;
    unsigned char key;
    
    char *input = malloc(sizeof(char));
    
    strcpy(input, orig);
    
    for (i = 0, key = 0; i < 256; ++i, ++key) {
        for (p = input; p < input + len; ++p) {
            *p ^= key;
        }
       
        if (string_decrypts(input, len)) {
            strcpy(orig, input);
            free(input);
            return key;
        }
         
        strcpy(input, orig);
    }
    
    free(input);
    return -1; 
}

bool string_decrypts(char *input, int len)
{
    int i, n;
    char *token = malloc(sizeof(char) * len);

    for (i = 0; i < len; i += n + 1) {
        sscanf(input, "%s", token);
        if (! hash_contains(token))
            return false;
        n = strlen(token);
        input += n + 1;
    }
    
    return true;
}

/* read_process_words */
void read_process_words(FILE *input, void do_work(char *t)) {
    int i;

    char *line, *token = NULL;
    char buf[LINE_BUFFER];
   
    size_t chars_read, buf_len;

    do {
        // reset variables
        line = NULL;
        line = (char *) malloc(sizeof(char));
        line[0] = '\0';
        buf_len = 0;

        buf[0] = '\0'; // Don't remember why this is necessary

        // read LINE_BUFFER bytes at a time, build up line
        do {
            fgets(buf, LINE_BUFFER, input);
            chars_read = strlen(buf);
            buf_len += chars_read;
            line = realloc(line, sizeof(char) * buf_len + 1);
            strcat(line, buf);
        } while (chars_read == LINE_BUFFER - 1);

        token = (char *) malloc(sizeof(char) * (buf_len + 1));

        int m, n;
        char *l = line;
        for (i = 0, m = strlen(line); i < m; i += n + 1) {
            sscanf(line, "%s", token);
            do_work(token);
            n = strlen(token);
            line += n + 1;
        }

        free(l);
    } while (chars_read);
}

/* init_hash */
void init_hash() {
    int i;

    for (i = 0; i < HASH_SIZE; ++i)
        hash_array[i] = NULL;
}

/* convert_to_key */
unsigned long convert_to_key(char *s) {
    char c;
    unsigned long key = 0;

    while ((c = *s++) != '\0')
        key = (key * 27) + (c - 'a') + 1;

    return key;
}

/* hash_contains */
bool hash_contains(char *s) {
    unsigned long key = convert_to_key(s);

    hash_node *p = hash_array[key % HASH_SIZE];

    while (p != NULL) {
        if (p->key == key)
            return true;
        p = p->next;
    }
 
    return false;
}
 
/* hash_insert */
void hash_insert(char *s) {
    unsigned long key = convert_to_key(s);

    hash_node *item = (hash_node*) malloc(sizeof(hash_node));
    item->key = key;
    item->next = NULL;
 
    hash_node *p = hash_array[key % HASH_SIZE];
 
    if (p == NULL)
        hash_array[key % HASH_SIZE] = item;
    else {
        while (p->next != NULL)
            p = p->next;
        p->next = item;
    }
}
