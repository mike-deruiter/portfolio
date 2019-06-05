// TODO: rewrite "print_heap()" to resemble the Unix utility "tree"

/* heap - simple program demonstrating an array-based heap for educational
 *        purposes. Not perfect -- the print_heap() method is a kludge.
 *
 * Copyright (c) 2017, Mike DeRuiter
 * send bugs to: aynrandjuggalo@gmail.com
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
#include <time.h>

/* this program is only able to print 7 or less nodes, so don't make this
   value bigger unless you rewrite the print_heap() function.             */
#define BUFFER_SIZE     50
#define BUFFER_INCREASE 25
#define NUMBERS_TO_ADD  7

void heap_insert(int n);
int heap_delete();
void rebuild_heap(int root);
void print_heap();

int *heap;
int size = 0;

int buffer_multiple = 0;

int main()
{
    // i is used for controlling loops, n is the next number to be inserted
    int i, n;

    heap = malloc(sizeof(int) * BUFFER_SIZE);

    srand(time(NULL)); // seed the random number generator

    printf("n: ");
    for (i = 0; i < NUMBERS_TO_ADD; ++i) {
        // generate a random number between 10 & 99 (always two characters wide)
        n = (rand() % 90) + 10; 
        printf("%2d ", n); // print the character
        heap_insert(n);    // ...& insert it into the heap
    }
    printf("\n\n");

    print_heap();
    putchar('\n'); // second newline

    // remove the top element of the heap
    n = heap_delete();

    printf("deleted: %d\n\n", n);

    print_heap();

    free(heap);

    return 0;
}

/* heap_insert - insert a new item into the heap. */
void heap_insert(int n)
{
    // resize the heap first if the heap is full.
    if (size >= BUFFER_SIZE)
        heap = realloc(heap, sizeof(int) * (BUFFER_SIZE + 
                       (BUFFER_INCREASE * ++buffer_multiple)));

    int i = size; // start with the bottom of the heap

    heap[i] = n;  // insert the new item at the bottom of the heap
 
    // find the parent node
    int parent = (i - 1) / 2;

    /* while the parent exists & the new item is greater, "bubble up" the
       item into its appropriate position.                                */
    while (parent >= 0 && heap[i] > heap[parent]) {
        int temp = heap[i];
        heap[i] = heap[parent];
        heap[parent] = temp;

        i = parent;
        parent = (i - 1) / 2;
    }

    ++size;
}

/* heap_delete - remove the top of the heap & return it. */
int heap_delete()
{
    // if the heap is empty, return 0.
    int item = 0;

    if (size > 0) {
        item = heap[0];

        // replace the top of the heap with the bottom.
        heap[0] = heap[size - 1];
        --size;
        // now that the bottom is at the top, rebuild starting from the top.
        rebuild_heap(0);
    }

    return item;
}

/* rebuild_heap - recursive method for rebuilding the heap starting at the
                  root.                                                    */
void rebuild_heap(int root)
{
    // check for the left child
    int child = (2 * root) + 1;

    // if (child < size), a left child exists.
    if (child < size) {
        int right_child = child + 1;
        
        // if a right child exists & is bigger than the left, use it instead.
        if (right_child < size && right_child > child)
            child = right_child;

        /* if the root is smaller than the biggest child, "bubble down" the
           root & rebuild the heap from its new position.                   */
        if (root < child) {
            int temp = heap[root];
            heap[root] = heap[child];
            heap[child] = temp;
            rebuild_heap(child);
        }
    }
}

/* print_heap - kludge method for printing the heap. */
void print_heap()
{
    // This method is messy & ugly, & only works w/ 7 digits or less.
    /* Ideally, I'd like for the output to look like the Unix utility "tree",
       but I don't know how to do that.                                       */          
    int i;

    if (size >= 1)
        printf("      %2d\n", heap[0]);
    if (size >= 2)
        printf("  %2d", heap[1]);
    if (size >= 3)
        printf("      %2d\n", heap[2]);
    if (size >= 4)
        printf("%2d", heap[3]);
    for (i = 4; i < size; ++i)
        printf("  %2d", heap[i]);
    putchar('\n');
}

