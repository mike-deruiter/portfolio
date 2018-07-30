/* bruteforce - encrypt input w/ single-byte XOR & then decrypt it w/ brute
   force. 
   
   TODO: Allow multi-line input                                             */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define HASH_SIZE   20
#define LINE_BUFFER 50

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
    
    len = getline(&line, &lb, stdin);
    // resize & chomp
    if (line[len] == '\n') {
        --len;
        line = realloc(line, sizeof(char) * len);
        line[len] = '\0';
    }
    
    if ((key = bruteforce(line, len)) != -1) {
        fprintf(stderr, "Key Found: %d\nDecrypted Message: %s\n", key, line);
    } else {
        fprintf(stderr, "%s: cannot decode message\n", argv[0]);
        exit(1);
    }
    
    exit(0);
}

int bruteforce(char *orig, int len)
{
    int i;
    char *p, *t;
    unsigned char key;
    
    char *input = malloc(sizeof(char));
    char *temp = malloc(sizeof(char));
    
    strcpy(input, orig);
    
    bool up = false, abend = false;
    
    for (i = 0, key = 0; i < 256; ++i, ++key) {
        strcpy(temp, input);
        
        for (p = input, t = temp; p < input + len; ++p, ++t) {
            *p ^= key;
            
            if (*p == '\0') {
                abend = true;
                break;
            }
            
            *t = *p;
            if (isupper(*p))
                *p = tolower(*p);
        }
        
        if (abend) {
            abend = false;
            continue;
        }
        
        if (string_decrypts(input, len)) {
            strcpy(orig, temp);
            free(temp);
            free(input);
            return key;
        }
         
        strcpy(input, orig);
    }
    
    free(temp);
    free(input);
    return -1; 
}

bool string_decrypts(char *input, int len)
{
    int i, j, n;
    char *token = malloc(sizeof(char) * len);

    for (i = 0; i < len; i += n + 1) {
        sscanf(input, "%s", token);
        if (! hash_contains(token))
            return false;
        n = strlen(token);
        input += n + 1;
    }
 
    free(token);
    
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
        while (sscanf(line, "%s", token) > 0) {
            do_work(token);
            line += strlen(token) + 1; 
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
