/* puzzle - Solves the following puzzle: Given the sequence of numbers
 * 1, 2, 3, 4, 5, 6, 7, 8, 9, how many ways can you insert a +, -, or nothing
 * between the digits to get a sum of 100? (Putting nothing between two
 * digits merges them into one number.)
 *  
 * Copyright (c) 2017, Mike DeRuiter. All rights reserved.
 * Send bugs to: aynrandjuggalo@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following condition is met:
 *   1) Redistributions of source code (if you're distributing a binary you
 *      don't have to bother. This source is potentially interesting for 
 *      educational purposes but the program itself is hardly the GNU 
 *      compiler) must retain the above copyright notice (please remove the 
 *      bit about sending bug reports to me), this list of one whole 
 *      condition (including the funny parts) & the following disclaimer:
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ''AS IS'' AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY 
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE.                                                               
 *
 * THIS DISCLAIMER HAS SUPER COW POWERS. (You have to include this part too
 * b/c it's funny to me)                                                      */ 

#include <stdio.h>
#include <stdlib.h>

// if we're using c99 or later, include stdbool.h; otherwise, roll our own
#if __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#else
typedef enum { false, true } bool;
#endif

// define the target sum & the number of digits.
#define SUM         100
#define NUM_DIGITS  9

/* I could have picked any code for the two operators, but these
   ones have the interesting property that multiplying them by -1
   gives you the original symbol.                                   */
typedef enum { NONE, ADD = ('+' * -1), SUB = ('-' * -1) } op_type;

// function protypes
void solve_puzzle(int pos);
void place_operator(int pos, op_type op);
void init_operators();
void gen_postfix();
int eval_postfix();
void print_expression();
bool is_operator(int i);

// global variables
op_type operators[NUM_DIGITS - 1]; 

// stack data structure, functions, & variables
typedef struct {
    int *stack;
    int top;
} stack;

stack *init_stack(int size);
void push(stack *s, int i);
int pop(stack *s);
int peek(stack *s);

stack *postfix;

// main method
int main() {
    postfix = init_stack(2*NUM_DIGITS - 1);
    init_operators();

    solve_puzzle(0);
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
    
/* init_operators */
void init_operators() {
    int i;

    // initialize the array of operators.
    for (i = 0; i < NUM_DIGITS - 1; ++i)
        operators[i] = NONE;
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
                if (is_operator(temp))                
                    push(postfix, temp);
        }
}

/* eval_postfix - pop expressions off the stack & evaluate. */
int eval_postfix() {
    int a;
    int i = pop(postfix);

    switch (i) {
        case ADD:
            /* we can reduce the recursive calls if we assume the next item
               in the stack is a number.                                    */
            a = pop(postfix);
            return eval_postfix() + a;
        case SUB:
            a = pop(postfix);
            return eval_postfix() - a;
        default:
            return i;
    }
}

/* print_expression - print the expression. */
void print_expression() {
    int i;

    putchar('1');
    for (i = 0; i < NUM_DIGITS - 1; ++i) {
        if (operators[i] != NONE)
            // remember how the codes have this interesting property?
            putchar(operators[i] * -1);
        putchar('2' + i);
    }
    putchar('\n');
}

/* is_operator - determine whether or not an item in the stack is an 
 * operator.                                                        */
bool is_operator(int i) {
    if (i == ADD || i == SUB)
        return true;

    return false;
}

/* init_stack */
stack *init_stack(int size) {
    stack *s;

    s = malloc(sizeof(stack));
    s->stack = malloc(size * sizeof(int));
    s->top = -1;

    return s;
}

/* pf_push - push a value onto the stack. */
void push(stack *s, int i) {
    s->stack[++s->top] = i;
}

/* pf_pop - pop a value from the stack. */
int pop(stack *s) {
    return s->stack[s->top--];
}

/* pf_peek - return the value on top of the stack. */
int peek(stack *s) {
    return s->stack[s->top];
}
