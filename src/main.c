#include "dijkstra.h"

#define COLOR_START "#03FCC69F"
#define COLOR_END   "#806EF59F"
#define COLOR_PATH  "#FC350390"

#define INDENT      "    "
#define FILLED_NODE "\"%c\"[style=\"filled\", fillcolor=\"%s\"]\n"

STATIC_ASSERT(CAP_VERTICES <= 64, "64 < CAP_VERTICES");
static void show(Memory* memory, u8 start, u8 end) {
    printf("graph {\n" INDENT FILLED_NODE INDENT FILLED_NODE,
           FROM_INDEX(start),
           COLOR_START,
           FROM_INDEX(end),
           COLOR_END);
    u64 visited[CAP_NODES] = {0};
    for (u8 i = end; i != start;) {
        printf(INDENT "\"%c\" -- \"%c\""
                      "[label=\"%hhu\", penwidth=5.0, color=\"%s\"]\n",
               FROM_INDEX(i),
               FROM_INDEX(memory->vertices[i].previous),
               memory->distance[i][memory->vertices[i].previous],
               COLOR_PATH);
        if (i != end) {
            printf(INDENT FILLED_NODE, FROM_INDEX(i), COLOR_PATH);
        }
        visited[i] |= 1lu << memory->vertices[i].previous;
        visited[memory->vertices[i].previous] |= 1lu << i;
        i = memory->vertices[i].previous;
    }
    for (u8 i = 0; i < CAP_VERTICES; ++i) {
        for (u8 j = i + 1; j < CAP_VERTICES; ++j) {
            if (((visited[i] >> j) & 1lu) || ((visited[j] >> i) & 1lu) ||
                (!memory->distance[i][j]))
            {
                continue;
            }
            printf(INDENT "\"%c\" -- \"%c\"[label=\"%hhu\"]\n",
                   FROM_INDEX(i),
                   FROM_INDEX(j),
                   memory->distance[i][j]);
        }
    }
    printf("}\n");
}

i32 main(void) {
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
        u8 start = INTO_INDEX('A');
        u8 end = INTO_INDEX('E');
        dijkstra(memory, start, end);
        show(memory, start, end);
    }
    fprintf(stderr, "Done!\n");
    free(memory);
    return EXIT_SUCCESS;
}
