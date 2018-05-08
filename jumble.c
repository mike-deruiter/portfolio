/* jumble - takes jumbled-up words from stdin & figures out if they 
   unscramble to any words in the system's dictionary (/usr/share/dict/words) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// if we're using c99 or later, include stdbool.h; otherwise, roll our own
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef enum { false, true } bool;
#endif

// macros
#define DICT        "/usr/share/dict/words"
#define HASH_SIZE   20
#define LINE_BUFFER 500

// function prototypes
void read_process_words(FILE *input, void do_work(char *t));
void insert_word(char *token);
void lookup_word(char *token);

// hash table functions
void init_hash();
unsigned long convert_to_key(char *s);
int hash_code(unsigned long key);
bool hash_contains(char *s);
void hash_insert(char *s);

void sort(char a[], int s);
void strip_path(char **s);

typedef struct hash_node {
    long key;
    struct hash_node *next;
} hash_node;

hash_node *hash_array[HASH_SIZE];

int main(int argc, char *argv[]) {
    FILE *input;

    init_hash();

    // remove leading ".*/" from argv[0], becomes just the program name
    strip_path(&argv[0]); 

    if (argc > 1) {
        int i;
        for (i = 1; argc-- > 1; ++i) {
            input = fopen(argv[i], "r");

            if (input == NULL) {
                printf("%s: cannot open %s\n", argv[0], argv[i]);
                return 1;
            }

            read_process_words(input, insert_word);
            fclose(input);
        }
    } else
        read_process_words(stdin, insert_word);

    FILE *dict = fopen(DICT, "r");

    read_process_words(dict, lookup_word);
    
    fclose(dict);

    return 0;
}

/* read_process_words - read a line of arbitray length & break it up into
   tokens (words). Perform work on each token                             */
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

        buf[0] = '\0'; // don't remember why this is necessary

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

/* insert_word */
void insert_word(char *token) {
    char *p;
    int l = strlen(token);
    
    for (p = token; p < token + l; ++p)
        *p = tolower(*p);

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

/* sort */
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
        // swap
        temp = a[min]; a[min] = a[i]; a[i] = temp;
    }
}

/* strip_path */
void strip_path(char **name) {
    int i;

    for (i = strlen(*name); i > 0 && (*name)[i - 1] != '/'; --i)
        ;

    *name += i;
}
