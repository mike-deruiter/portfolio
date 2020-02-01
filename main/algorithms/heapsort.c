// TODO: rewrite "print_heap()" to resemble the Unix utility "tree"

/* heap - simple program demonstrating an array-based heap for educational
 *        purposes. Not perfect -- the print_heap() method is a kludge. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "heap.h"

/* this program is only able to print 7 or less nodes, so don't make this
   value bigger unless you rewrite the print_heap() function.             */
#define NUMBERS_TO_ADD  7

void print_heap(Heap *heap);

int main()
{
    // i is used for controlling loops, n is the next number to be inserted
    int i, n;
    Heap *heap;

    heap = init_heap();

    srand(time(NULL)); // seed the random number generator

    for (i = 0; i < NUMBERS_TO_ADD; ++i) {
        // generate a random number between 10 & 99 (always two characters wide)
        n = (rand() % 90) + 10; 
        printf("%2d ", n); // print the character
        heap_insert(heap, n);    // ...& insert it into the heap
    }
    printf("\n\n");

    print_heap(heap);
    putchar('\n'); // second newline

    // remove the top element of the heap
    n = heap_delete(heap);

    printf("deleted: %d\n\n", n);

    print_heap(heap);

    free(heap);

    return 0;
}

/* print_heap - kludge method for printing the heap. */
void print_heap(Heap *heap)
{
    // This method is messy & ugly, & only works w/ 7 digits or less.
    /* Ideally, I'd like for the output to look like the Unix utility "tree",
       but I don't know how to do that.                                       */          
    int i;

    if (heap->size >= 1)
        printf("      %2d\n", heap->stack[0]);
    if (heap->size >= 2)
        printf("  %2d", heap->stack[1]);
    if (heap->size >= 3)
        printf("      %2d\n", heap->stack[2]);
    if (heap->size >= 4)
        printf("%2d", heap->stack[3]);
    for (i = 4; i < heap->size; ++i)
        printf("  %2d", heap->stack[i]);
    putchar('\n');
}
