typedef struct _node {
    int val;
    struct _node *next;
} Node;

Node *init_list();
void list_cleanup();
