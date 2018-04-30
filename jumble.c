/* jumble - takes jumbled-up words from stdin & figures out if they 
   unscramble to any words in the system's dictionary (/usr/share/dict/words) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// if we're using c99 or later, include stdbool.h; otherwise, roll our own
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef enum { false, true } bool;
#endif

// macros
#define DICT        "/usr/share/dict/words"
#define SIZE        20
#define LINE_BUFFER 500

// function prototypes
void read_process_words(FILE *input, void do_work(char *t));
void insert_word(char *token);
void lookup_word(char *token);

// hash table functions
void initHash();
unsigned long convert_to_key(char *s);
int hash_code(unsigned long key);
bool hash_contains(char *s);
void hash_insert(char *s);

void sort(char a[], int s);

typedef struct hash_node {
    long key;
    struct hash_node *next;
} HashNode;

HashNode *hashArray[SIZE];

int main(int argc, char *argv[]) {
    int i;

    initHash();

    if (argc == 1) {
        fprintf(stderr, "jumble: 1 or more arguments required\n");
    } else {
        for (i = 1; i < argc; ++i) {
            insert_word(argv[i]);
        }
    }

    FILE *dict = fopen(DICT, "r");

    read_process_words(dict, lookup_word);
    
    fclose(dict);

    return 0;
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

/* insert_word */
void insert_word(char *token) {
    sort(token, strlen(token));
    hash_insert(token);
}

/* lookup_word */
void lookup_word(char *token) {
    int t = strlen(token);
    char *output = (char *) malloc(sizeof(char) * (t + 1));

    output = strcpy(output, token);
    sort(token, t);
    if (hash_contains(token))
        printf("%s\n", output);
}


void initHash() {
    int i;

    for (i = 0; i < SIZE; ++i)
        hashArray[i] = NULL;
}

unsigned long convert_to_key(char *s) {
    char c;
    long key = 0;

    while ((c = *s++) != '\0')
        key = (key * 27) + (c - 'a') + 1;

    return key;
}

int hash_code(unsigned long key) {
    return key % SIZE;
}

bool hash_contains(char *s) {
    unsigned long key = convert_to_key(s);

    HashNode *p = hashArray[hash_code(key)];

    while (p != NULL) {
        if (p->key == key)
            return true;
        p = p->next;
    }
 
    return false;
}
 
void hash_insert(char *s) {
    unsigned long key = convert_to_key(s);

    HashNode *item = (HashNode*) malloc(sizeof(HashNode));
    item->key = key;
    item->next = NULL;
 
    HashNode *p = hashArray[hash_code(key)];
 
    if (p == NULL)
        hashArray[hash_code(key)] = item;
    else {
        while (p->next != NULL)
            p = p->next;
        p->next = item;
    }
}

void sort(char a[], int sz)
{
    // selection sort
    int i, j, min, temp;

    for (i = 0; i < sz-1; ++i) {
        min = i;
        for (j = i+1; j < sz; ++j) {
            if (a[j] < a[min]) {
                min = j;
            }
        }
        temp = a[min]; a[min] = a[i]; a[i] = temp;
    }
}
