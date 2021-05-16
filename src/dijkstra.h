#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include "heap.h"

#define CAP_VERTICES 64

typedef struct {
    u8 distance;
    u8 previous;
} Vertex;

typedef struct {
    u8     distance[CAP_VERTICES][CAP_VERTICES];
    Vertex vertices[CAP_VERTICES];
    Heap   unvisited;
} Memory;

STATIC_ASSERT(CAP_VERTICES <= 64, "64 < CAP_VERTICES");
static void dijkstra(Memory* memory, u8 start, u8 end) {
    for (u8 i = 0; i < CAP_VERTICES; ++i) {
        if (i == start) {
            memory->vertices[i].distance = 0;
        } else {
            memory->vertices[i].distance = MAX_U8;
        }
    }
    u64 visited = 0;
    insert(&memory->unvisited, (Node){.vertex = start, .priority = 0});
    while (memory->unvisited.len_nodes != 0) {
        Node node = pop(&memory->unvisited);
        if (node.vertex == end) {
            break;
        }
        for (u8 i = 0; i < CAP_VERTICES; ++i) {
            if (!memory->distance[node.vertex][i]) {
                continue;
            }
            u8 distance = memory->vertices[node.vertex].distance +
                          memory->distance[node.vertex][i];
            if (distance < memory->vertices[i].distance) {
                memory->vertices[i].distance = distance;
                memory->vertices[i].previous = node.vertex;
                if (!((visited >> node.vertex) & 1lu)) {
                    drop(&memory->unvisited, i);
                    insert(&memory->unvisited,
                           (Node){.vertex = i, .priority = distance});
                }
            }
        }
        visited |= 1lu << node.vertex;
    }
    return;
}

#define INTO_INDEX(char_) ((u8)(char_ - 'A'))
#define FROM_INDEX(index) ((char)('A' + index))
#define SET_DISTANCE(memory, from, to, distance_)             \
    {                                                         \
        u8 i = INTO_INDEX(from);                              \
        u8 j = INTO_INDEX(to);                                \
        EXIT_IF((CAP_VERTICES <= i) || (CAP_VERTICES <= j) || \
                (distance_ == MAX_U8));                       \
        memory->distance[i][j] = distance_;                   \
        memory->distance[j][i] = distance_;                   \
    }

#endif
