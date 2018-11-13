/* puzzle - Solves the following puzzle: Given the sequence of numbers
   1, 2, 3, 4, 5, 6, 7, 8, 9, how many ways can you insert a +, -, or nothing
   between the digits to get a sum of 100? (Putting nothing between two
   digits merges them into one number.)                                       */

#include <stdio.h>
#include <stdlib.h>

// if we're using c99 or later, include stdbool.h; otherwise, roll our own
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef enum { false, true } bool;
#endif

#define SUM         100
#define NUM_DIGITS  9

/* I could have picked any code for the two operators, but these
   ones have the interesting property that multiplying them by -1
   gives you the original symbol.                                   */
typedef enum { NONE, ADD = ('+' * -1), SUB = ('-' * -1) } op_type;

// function protypes
void init_operators();
void solve_puzzle(int pos);
void place_operator(int pos, op_type op);
void gen_postfix();
int  eval_postfix();
void print_expression();
bool is_operator(int i);

// global variables
op_type operators[NUM_DIGITS - 1]; 

// stack data structure, functions, & variable
typedef struct {
    int *stack;
    int top;
} Stack;

Stack *init_stack(int size);
void push(Stack *s, int i);
int pop(Stack *s);
int peek(Stack *s);

Stack *postfix;

/* main */
int main() {
    postfix = init_stack(2*NUM_DIGITS - 1);
    init_operators();

    solve_puzzle(0);
}

/* init_operators */
void init_operators() {
    int i;

    for (i = 0; i < NUM_DIGITS - 1; ++i)
        operators[i] = NONE;
}

/* solve_puzzle - recursive method for solving the puzzle. */
void solve_puzzle(int pos) {
    if (pos >= NUM_DIGITS - 1)
        return;
    
    place_operator(pos, ADD);
    place_operator(pos, SUB);
    place_operator(pos, NONE);
}

/* place_operator - place a symbol at the given position. If the resulting
 * expression isn't correct, keep the symbol & try solving the rest of the 
 * puzzle at the next position.                                            */ 
void place_operator(int pos, op_type op) {
    operators[pos] = op;
    gen_postfix();
    if (eval_postfix() == SUM) {
        print_expression();
    } else
        solve_puzzle(pos + 1);
}
    

/* gen_postfix - create the postfix stack for evaluating the expression. */
void gen_postfix() {
    int i;
   
    // push the leading 1 onto the stack.
    push(postfix, 1);
    // giving the temp variable a default value safeguards against leading NONEs
    int temp = NONE; 
    for (i = 0; i < NUM_DIGITS - 1; ++i)
        switch (operators[i]) {
            case ADD:                 
            case SUB:
                // push the next digit & the operator onto the stack.
                push(postfix, 2 + i);
                push(postfix, operators[i]);
                break;            
            default:
                /* if the top of the stack is an operator, pop it & 
                   remember it (it's always an operator unless there are 
                   leading NONEs)                                        */
                if (is_operator(peek(postfix)))
                    temp = pop(postfix);
                // merge the top of the stack with the new digit 
                push(postfix, (pop(postfix) * 10) + (2 + i));
                // ...then put the operator back
                if (temp != NONE)                
                    push(postfix, temp);
        }
}

/* eval_postfix - pop expressions off the stack & evaluate. */
int eval_postfix() {
    int a;
    int p = pop(postfix);

    switch (p) {
        case ADD:
            /* we can reduce the recursive calls if we assume the next item
               in the stack is a number.                                    */
            a = pop(postfix);
            return eval_postfix() + a;
        case SUB:
            a = pop(postfix);
            return eval_postfix() - a;
        default:
            return p;
    }
}

/* print_expression - print the expression. */
void print_expression() {
    int i;

    putchar('1');
    for (i = 0; i < NUM_DIGITS - 1; ++i) {
        if (operators[i] != NONE)
            putchar(operators[i] * -1);
        putchar('2' + i);
    }
    putchar('\n');
}

/* is_operator - determine whether or not an item in the stack is an 
 * operator.                                                         */
bool is_operator(int i) {
    if (i == ADD || i == SUB)
        return true;

    return false;
}

/* init_stack */
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
