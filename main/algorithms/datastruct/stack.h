#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef enum { false, true } bool;
#endif

typedef struct {
    int *stack;
    int top;
} Stack;

Stack *init_stack(int size);
void push(Stack *s, int i);
int pop(Stack *s);
int peek(Stack *s);
bool stack_empty(Stack *s);
