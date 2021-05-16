#include "dijkstra.h"

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
