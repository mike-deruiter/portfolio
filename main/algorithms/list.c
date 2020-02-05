#include "list.h"
#include <stdlib.h>

Node *z;

Node *init_list() {
    Node *head;
	
    if (z == NULL) {
        z = malloc(sizeof(Node *));
        z->next = z;
    }

    head = malloc(sizeof(Node *));
    head->next = z;

    return head;
}

void list_cleanup() {
    free(z);
}
