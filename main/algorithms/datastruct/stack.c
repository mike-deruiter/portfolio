#include "stack.h"
#include <stdlib.h>

/* init_stack - create stack object */
Stack *init_stack(int size) {
    Stack *s;

    s = malloc(sizeof(Stack));
    s->stack = malloc(size * sizeof(int));
    s->top = -1;

    return s;
}

/* push - push a value onto the stack. */
void push(Stack *s, int i) {
    s->stack[++s->top] = i;
}

/* pop - pop a value from the stack. */
int pop(Stack *s) {
    return s->stack[s->top--];
}

/* peek - return the value on top of the stack. */
int peek(Stack *s) {
    return s->stack[s->top];
}

/* stack_empty - returns true if stack is empty */
bool stack_empty(Stack *s) {
    return s->top == -1 ? true : false;
}
