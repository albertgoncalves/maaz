#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CAP_NODES 64
#define CAP_EDGES 64

typedef uint8_t u8;

typedef int32_t i32;

typedef struct {
    const char* label;
} Node;

typedef struct {
    Node* from;
    Node* to;
} Edge;

typedef struct {
    Node nodes[CAP_NODES];
    u8   len_nodes;
    Edge edges[CAP_EDGES];
    u8   len_edges;
} Memory;

#define EXIT_IF(condition)           \
    if (condition) {                 \
        fprintf(stderr,              \
                "\n%s:%s:%d `%s`\n", \
                __FILE__,            \
                __func__,            \
                __LINE__,            \
                #condition);         \
        exit(EXIT_FAILURE);          \
    }

static Node* alloc_node(Memory* memory) {
    EXIT_IF(CAP_NODES <= memory->len_nodes);
    return &memory->nodes[memory->len_nodes++];
}

static Edge* alloc_edge(Memory* memory) {
    EXIT_IF(CAP_EDGES <= memory->len_edges);
    return &memory->edges[memory->len_edges++];
}

#define N 5

#define ALLOC_EDGE(memory, i, j)         \
    {                                    \
        Edge* edge = alloc_edge(memory); \
        edge->from = nodes[i];           \
        edge->to = nodes[j];             \
    }

i32 main(void) {
    fprintf(stderr,
            "\n"
            "sizeof(Node)   : %zu\n"
            "sizeof(Edge)   : %zu\n"
            "sizeof(Memory) : %zu\n"
            "\n",
            sizeof(Node),
            sizeof(Edge),
            sizeof(Memory));
    Memory* memory = calloc(1, sizeof(Memory));
    EXIT_IF(!memory);
    {
        Node* nodes[N];
        for (u8 i = 0; i < N; ++i) {
            nodes[i] = alloc_node(memory);
        }
        nodes[0]->label = "a";
        nodes[1]->label = "b";
        nodes[2]->label = "c";
        nodes[3]->label = "d";
        nodes[4]->label = "e";
        ALLOC_EDGE(memory, 0, 1);
        ALLOC_EDGE(memory, 0, 2);
        ALLOC_EDGE(memory, 0, 4);
        ALLOC_EDGE(memory, 1, 0);
        ALLOC_EDGE(memory, 1, 2);
        ALLOC_EDGE(memory, 2, 0);
        ALLOC_EDGE(memory, 3, 0);
        ALLOC_EDGE(memory, 4, 2);
        ALLOC_EDGE(memory, 4, 3);
    }
    {
        printf("digraph {\n");
        for (u8 i = 0; i < memory->len_edges; ++i) {
            Edge edge = memory->edges[i];
            EXIT_IF((!edge.from) || (!edge.to));
            printf("    \"%s\" -> \"%s\"", edge.from->label, edge.to->label);
        }
        printf("}\n");
    }
    fprintf(stderr, "Done!\n");
    free(memory);
    return EXIT_SUCCESS;
}
