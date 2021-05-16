#include "heap.h"

Bool check(Heap*, u8);
Bool check(Heap* heap, u8 i) {
    if (heap->len_nodes <= i) {
        return TRUE;
    }
    u8 l = GET_LEFT_CHILD(i);
    u8 r = l + 1;
    if ((l < heap->len_nodes) &&
        (heap->nodes[l].priority < heap->nodes[i].priority))
    {
        return FALSE;
    }
    if ((r < heap->len_nodes) &&
        (heap->nodes[r].priority < heap->nodes[i].priority))
    {
        return FALSE;
    }
    return check(heap, l) && check(heap, r);
}

#define INSERT(heap, x)                                   \
    {                                                     \
        insert(heap, (Node){.vertex = x, .priority = x}); \
        EXIT_IF(!check(heap, 0));                         \
    }

#define DROP(heap, x)             \
    {                             \
        drop(heap, x);            \
        EXIT_IF(!check(heap, 0)); \
    }

i32 main(void) {
    fprintf(stderr,
            "sizeof(Node) : %zu\n"
            "sizeof(Heap) : %zu\n",
            sizeof(Node),
            sizeof(Heap));
    Heap* heap = calloc(1, sizeof(Heap));
    EXIT_IF(!heap);
    {
        INSERT(heap, 8);
        INSERT(heap, 7);
        DROP(heap, 7);
        DROP(heap, 8);
        INSERT(heap, 7);
        INSERT(heap, 8);
        INSERT(heap, 1);
        INSERT(heap, 4);
        INSERT(heap, 1);
        DROP(heap, 1);
        INSERT(heap, 3);
        INSERT(heap, 12);
        INSERT(heap, 20);
        INSERT(heap, 4);
        INSERT(heap, 20);
        DROP(heap, 4);
        INSERT(heap, 19);
        INSERT(heap, 3);
        DROP(heap, 3);
        INSERT(heap, 0);
        DROP(heap, 20);
        DROP(heap, 20);
        INSERT(heap, 20);
        INSERT(heap, 0);
        INSERT(heap, 25);
        DROP(heap, 0);
        INSERT(heap, 200);
        DROP(heap, 12);
        INSERT(heap, 200);
        DROP(heap, 0);
        INSERT(heap, 1);
    }
    {
        u8 previous = 0;
        while (heap->len_nodes != 0) {
            u8 current = pop(heap).priority;
            EXIT_IF((!check(heap, 0)) || (current < previous));
            previous = current;
            fprintf(stderr, ".");
        }
        fprintf(stderr, "\n");
    }
    free(heap);
    return EXIT_SUCCESS;
}
