/* puzzle - Solves the following puzzle: Given the sequence of numbers
 * 1, 2, 3, 4, 5, 6, 7, 8, 9, how many ways can you insert a +, -, or nothing
 * between the digits to get a sum of 100? (Putting nothing between two
 * digits merges them into one number.) [C++ Version]
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

#include <iostream>
#include <stdexcept>
using namespace std;

// define the target sum & the number of digits.
constexpr int SUM = 100;
constexpr int NUM_DIGITS = 9;

/* I could have picked any code for the two operators, but these
   ones have the interesting property that multiplying them by -1
   gives you the original symbol.                                   */
typedef enum { NONE, ADD = ('+' * -1),  SUB = ('-' * -1) } op_type;

// function protypes
void init_operators();
void solve_puzzle();
void solve_puzzle(int pos);
void place_operator(int pos, op_type op);
void gen_postfix();
int eval_postfix();
void print_expression();
bool is_operator(int i);

// class for storing the postfix expression
class int_array_stack {
    private:
        int* stack;
        int top;
        int size;
    public:
        int_array_stack(int s) {
            size = s;
            stack = new int[size];
            top = -1;
        }

        int_array_stack() {
            int_array_stack(10);
        }

        void push(int i) { 
            if (top >= size)
                throw out_of_range("Postfix stack is full.");
            stack[++top] = i; 
        }

        int pop() { 
            if (top < 0)
                throw out_of_range("Postfix stack is empty.");
            return stack[top--]; 
        }

        int peek() const { return stack[top]; }
};

// global variables
op_type operators[NUM_DIGITS - 1];
int_array_stack postfix((NUM_DIGITS * 2) - 1);

// main method
int main() {
    init_operators();
 
    try {
        solve_puzzle();
    } catch (const out_of_range oor) {
        cerr << "ERROR: " << oor.what() << endl;
    }
}

/* init_operators */
void init_operators() {
    for (int i = 0; i < NUM_DIGITS - 1; ++i)
        operators[i] = NONE;
}

/* solve_puzzle (no arguments) - calls the recursive version of the function
   starting at position 0.                                                   */
void solve_puzzle() {
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
    
/* gen_postfix - create the postfix stack for evaluating the expression. */
void gen_postfix() {
    int digit = 1;
    // push the leading 1 onto the stack.
    postfix.push(digit++);
    
    int temp = NONE; // giving the temp variable a default value helps later 
    for (int op : operators)
        switch (op) {
            case ADD:                 
            case SUB:
                // push the next digit & the operator onto the stack.
                postfix.push(digit++);
                postfix.push(op);
                break;            
            default:
                /* if the top of the stack is an operator, pop it & 
                   remember it (it's always an operator unless there are 
                   leading NONEs -- NONE is not an operator)             */
                if (is_operator(postfix.peek()))
                    temp = postfix.pop();
                // merge the top of the stack with the new digit 
                postfix.push((postfix.pop() * 10) + (digit++));
                /* ...then put the operator back */
                if (is_operator(temp))                
                    postfix.push(temp);
        }
}

/* eval_postfix - pop expressions off the stack & evaluate. */
int eval_postfix() {
    int a;
    int i = postfix.pop();

    switch (i) {
        case ADD:
            /* we can reduce the recursive calls if we assume the next item
               in the stack is a number.                                    */
            a = postfix.pop();
            return eval_postfix() + a;
        case SUB:
            a = postfix.pop();
            return eval_postfix() - a;
        default:
            return i;
    }
}

/* print_expression - print the expression. */
void print_expression() {
    int digit = 1;

    cout << (char)('0' + digit++);
    for (int op : operators) {
        if (op != NONE)            
            // remember how the codes have this interesting property?
            cout << (char)(op * -1);
        cout << (char)('0' + digit++);
    }
    cout << endl;
}

/* is_operator - determine whether or not an item in the stack is an 
 * operator.                                                        */
bool is_operator(int i) {
    if (i == ADD || i == SUB)
        return true;

    return false;
}
