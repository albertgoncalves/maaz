#ifndef __HEAP_H__
#define __HEAP_H__

#include "prelude.h"

#define CAP_NODES 64

typedef struct {
    u8 vertex;
    u8 priority;
} Node;

typedef struct {
    Node nodes[CAP_NODES];
    u8   len_nodes;
} Heap;

#define SWAP(nodes, i, j)    \
    {                        \
        Node t = nodes[i];   \
        nodes[i] = nodes[j]; \
        nodes[j] = t;        \
    }

static void balance_up(Heap* heap, u8 i) {
    u8 j = (u8)(((i + 1) / 2) - 1);
    while (0 < i) {
        if (heap->nodes[i].priority < heap->nodes[j].priority) {
            SWAP(heap->nodes, i, j);
        }
        i = j;
        j = (u8)(((i + 1) / 2) - 1);
    }
}

static void balance_down(Heap* heap, u8 i) {
    for (;;) {
        u8 l = (u8)(((i + 1) * 2) - 1);
        u8 r = l + 1;
        u8 m = i;
        if ((l < heap->len_nodes) &&
            (heap->nodes[l].priority < heap->nodes[m].priority))
        {
            m = l;
        }
        if ((r < heap->len_nodes) &&
            (heap->nodes[r].priority < heap->nodes[m].priority))
        {
            m = r;
        }
        if (i == m) {
            return;
        }
        SWAP(heap->nodes, i, m);
        i = m;
    }
}

static void insert(Heap* heap, Node node) {
    EXIT_IF(CAP_NODES <= heap->len_nodes);
    u8 n = heap->len_nodes;
    heap->nodes[heap->len_nodes++] = node;
    balance_up(heap, n);
}

static Node pop(Heap* heap) {
    EXIT_IF(heap->len_nodes == 0);
    Node node = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->len_nodes];
    balance_down(heap, 0);
    return node;
}

static void drop(Heap* heap, u8 vertex) {
    if (heap->len_nodes == 0) {
        return;
    }
    u8 i = 0;
    for (; i < heap->len_nodes; ++i) {
        if (heap->nodes[i].vertex == vertex) {
            break;
        }
    }
    if (i == heap->len_nodes) {
        return;
    }
    u8 previous = heap->nodes[i].priority;
    heap->nodes[i] = heap->nodes[--heap->len_nodes];
    if (previous < heap->nodes[i].priority) {
        balance_down(heap, i);
    } else {
        balance_up(heap, i);
    }
}

#endif
