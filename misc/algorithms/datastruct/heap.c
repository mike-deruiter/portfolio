//TODO: Refactor HEAP_BUFFER_SIZE

#include "heap.h"
#include <stdlib.h>

Heap *init_heap() {
    Heap *h;

    h = malloc(sizeof(Heap));
    h->stack = malloc(HEAP_BUFFER_SIZE * sizeof(int));
    h->size = 0;
    h->_buf_m = 0;

    return h;
}

/* heap_insert - insert a new item into the heap. */
void heap_insert(Heap *heap, int n)
{
    // resize the heap first if the heap is full.
    if (heap->size >= HEAP_BUFFER_SIZE)
        heap->stack = realloc(heap->stack, sizeof(int) * (HEAP_BUFFER_SIZE + 
                              (HEAP_BUFFER_INCREASE * ++(heap->_buf_m))));

    int i = heap->size; // start with the bottom of the heap

    heap->stack[i] = n;  // insert the new item at the bottom of the heap
 
    // find the parent node
    int parent = (i - 1) / 2;

    /* while the parent exists & the new item is greater, "bubble up" the
       item into its appropriate position.                                */
    while (parent >= 0 && heap->stack[i] > heap->stack[parent]) {
        int temp = heap->stack[i];
        heap->stack[i] = heap->stack[parent];
        heap->stack[parent] = temp;

        i = parent;
        parent = (i - 1) / 2;
    }

    ++(heap->size);
}

/* heap_delete - remove the top of the heap & return it. */
int heap_delete(Heap *heap)
{
    // if the heap is empty, return 0.
    int item = 0;

    if (heap->size > 0) {
        item = heap->stack[0];

        // replace the top of the heap with the bottom.
        heap->stack[0] = heap->stack[heap->size - 1];
        --(heap->size);
        // now that the bottom is at the top, rebuild starting from the top.
        rebuild_heap(heap, 0);
    }

    return item;
}

/* rebuild_heap - recursive method for rebuilding the heap starting at the
                  root.                                                    */
void rebuild_heap(Heap *heap, int root)
{
    // check for the left child
    int child = (2 * root) + 1;

    // if (child < size), a left child exists.
    if (child < heap->size) {
        int right_child = child + 1;
        
        // if a right child exists & is bigger than the left, use it instead.
        if (right_child < heap->size && right_child > child)
            child = right_child;

        /* if the root is smaller than the biggest child, "bubble down" the
           root & rebuild the heap from its new position.                   */
        if (root < child) {
            int temp = heap->stack[root];
            heap->stack[root] = heap->stack[child];
            heap->stack[child] = temp;
            rebuild_heap(heap, child);
        }
    }
}
