#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STATIC_ASSERT _Static_assert

#define CAP_VERTICES 64

typedef uint8_t  u8;
typedef uint64_t u64;
typedef int32_t  i32;

#define MAX_U8 0xFF

typedef struct {
    u8 distance;
    u8 previous;
} Vertex;

typedef struct {
    u8 vertex;
    u8 priority;
} Node;

typedef struct {
    Node nodes[CAP_VERTICES];
    u8   len_nodes;
} Heap;

typedef struct {
    u8     distance[CAP_VERTICES][CAP_VERTICES];
    Vertex vertices[CAP_VERTICES];
    Heap   unvisited;
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

#define SWAP(nodes, i, j)    \
    {                        \
        Node t = nodes[i];   \
        nodes[i] = nodes[j]; \
        nodes[j] = t;        \
    }

static void insert(Heap* heap, Node node) {
    EXIT_IF(CAP_VERTICES <= heap->len_nodes);
    u8 i = heap->len_nodes;
    u8 j = (u8)(((i + 1) / 2) - 1);
    heap->nodes[heap->len_nodes++] = node;
    while (0 < i) {
        if (heap->nodes[i].priority < heap->nodes[j].priority) {
            SWAP(heap->nodes, i, j);
        }
        i = j;
        j = (u8)(((i + 1) / 2) - 1);
    }
}

static void balance(Heap* heap, u8 i) {
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

static Node pop(Heap* heap) {
    EXIT_IF(heap->len_nodes == 0);
    Node node = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->len_nodes];
    balance(heap, 0);
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
    heap->nodes[i] = heap->nodes[--heap->len_nodes];
    balance(heap, i);
}

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
    u8 steps = 0;
    while (memory->unvisited.len_nodes != 0) {
        Node node = pop(&memory->unvisited);
        if (node.vertex == end) {
            break;
        }
        ++steps;
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
    fprintf(stderr, "solved in `%hhu` steps\n", steps);
    return;
}

#define FROM_INDEX(index) ((char)('A' + index))

#define COLOR_START "#03FCC6FF"
#define COLOR_END   "#806EF5FF"
#define COLOR_PATH  "#FC350390"

static void show(Memory* memory, u8 start, u8 end) {
    printf("graph {\n"
           "    \"%c\"[style=\"filled\", fillcolor=\"%s\"]\n"
           "    \"%c\"[style=\"filled\", fillcolor=\"%s\"]\n",
           FROM_INDEX(start),
           COLOR_START,
           FROM_INDEX(end),
           COLOR_END);
    for (u8 i = 0; i < CAP_VERTICES; ++i) {
        for (u8 j = i + 1; j < CAP_VERTICES; ++j) {
            if (!memory->distance[i][j]) {
                continue;
            }
            printf("    \"%c\" -- \"%c\"[label=\"%hhu\"]\n",
                   FROM_INDEX(i),
                   FROM_INDEX(j),
                   memory->distance[i][j]);
        }
    }
    for (u8 i = end; i != start;) {
        Vertex vertex = memory->vertices[i];
        printf("    \"%c\" -- \"%c\"[penwidth=5.0, color=\"%s\"]\n",
               FROM_INDEX(i),
               FROM_INDEX(vertex.previous),
               COLOR_PATH);
        if (i != end) {
            printf("    \"%c\"[style=\"filled\", fillcolor=\"%s\"]\n",
                   FROM_INDEX(i),
                   COLOR_PATH);
        }
        i = vertex.previous;
    }
    printf("}\n");
}

#define INTO_INDEX(char_) ((u8)(char_ - 'A'))
#define SET_DISTANCE(memory, from, to, distance_)             \
    {                                                         \
        u8 i = INTO_INDEX(from);                              \
        u8 j = INTO_INDEX(to);                                \
        EXIT_IF((CAP_VERTICES <= i) || (CAP_VERTICES <= j) || \
                (distance_ == MAX_U8));                       \
        memory->distance[i][j] = distance_;                   \
        memory->distance[j][i] = distance_;                   \
    }

i32 main(void) {
    fprintf(stderr, "sizeof(Memory) : %zu\n", sizeof(Memory));
    Memory* memory = calloc(1, sizeof(Memory));
    EXIT_IF(!memory);
    {
        SET_DISTANCE(memory, 'A', 'B', 1);
        SET_DISTANCE(memory, 'A', 'D', 7);
        SET_DISTANCE(memory, 'B', 'D', 6);
        SET_DISTANCE(memory, 'A', 'C', 7);
        SET_DISTANCE(memory, 'C', 'E', 5);
        SET_DISTANCE(memory, 'D', 'E', 5);
        SET_DISTANCE(memory, 'A', 'F', 1);
        SET_DISTANCE(memory, 'F', 'G', 1);
        SET_DISTANCE(memory, 'G', 'H', 1);
        SET_DISTANCE(memory, 'D', 'H', 3);
        SET_DISTANCE(memory, 'A', 'I', 8);
        SET_DISTANCE(memory, 'I', 'D', 1);
        SET_DISTANCE(memory, 'A', 'J', 9);
        SET_DISTANCE(memory, 'J', 'H', 2);
    }
    {
        u8 start = INTO_INDEX('A');
        u8 end = INTO_INDEX('E');
        dijkstra(memory, start, end);
        show(memory, start, end);
    }
    fprintf(stderr, "Done!\n");
    free(memory);
    return EXIT_SUCCESS;
}
