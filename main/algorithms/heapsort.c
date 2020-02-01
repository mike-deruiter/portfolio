// TODO: rewrite "print_heap()" to resemble the Unix utility "tree"

/* heap - simple program demonstrating an array-based heap for educational
 *        purposes. Not perfect -- the print_heap() method is a kludge. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE     50
#define BUFFER_INCREASE 25
/* this program is only able to print 7 or less nodes, so don't make this
   value bigger unless you rewrite the print_heap() function.             */
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

