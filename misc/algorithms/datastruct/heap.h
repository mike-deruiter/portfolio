#define HEAP_BUFFER_SIZE     50
#define HEAP_BUFFER_INCREASE 25

typedef struct {
    int *stack;
    int size;
    int _buf_m;
} Heap;

Heap *init_heap();
void heap_insert(Heap *heap, int n);
int heap_delete(Heap *heap);
void rebuild_heap(Heap *heap, int root);
