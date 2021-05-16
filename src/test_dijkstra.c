#include <string.h>

#include "dijkstra.h"

#define TEST(memory, block)                \
    {                                      \
        memset(memory, 0, sizeof(Memory)); \
        block;                             \
        fprintf(stderr, ".");              \
    }

i32 main(void) {
    fprintf(stderr, "sizeof(Memory) : %zu\n", sizeof(Memory));
    Memory* memory = malloc(sizeof(Memory));
    EXIT_IF(!memory);
    TEST(memory, {
        SET_DISTANCE(memory, 'A', 'B', 4);
        SET_DISTANCE(memory, 'A', 'C', 1);
        SET_DISTANCE(memory, 'B', 'C', 2);
        u8 start = INTO_INDEX('A');
        u8 end = INTO_INDEX('B');
        dijkstra(memory, start, end);
        EXIT_IF(memory->vertices[INTO_INDEX('A')].distance != 0);
        EXIT_IF(memory->vertices[INTO_INDEX('B')].distance != 3);
        EXIT_IF(memory->vertices[INTO_INDEX('B')].previous != INTO_INDEX('C'));
        EXIT_IF(memory->vertices[INTO_INDEX('C')].distance != 1);
        EXIT_IF(memory->vertices[INTO_INDEX('C')].previous != INTO_INDEX('A'));
    });
    TEST(memory, {
        SET_DISTANCE(memory, 'A', 'B', 6);
        SET_DISTANCE(memory, 'A', 'D', 1);
        SET_DISTANCE(memory, 'B', 'C', 5);
        SET_DISTANCE(memory, 'B', 'D', 2);
        SET_DISTANCE(memory, 'B', 'E', 2);
        SET_DISTANCE(memory, 'C', 'E', 5);
        SET_DISTANCE(memory, 'D', 'E', 1);
        u8 start = INTO_INDEX('A');
        u8 end = INTO_INDEX('C');
        dijkstra(memory, start, end);
        EXIT_IF(memory->vertices[INTO_INDEX('A')].distance != 0);
        EXIT_IF(memory->vertices[INTO_INDEX('B')].distance != 3);
        EXIT_IF(memory->vertices[INTO_INDEX('B')].previous != INTO_INDEX('D'));
        EXIT_IF(memory->vertices[INTO_INDEX('C')].distance != 7);
        EXIT_IF(memory->vertices[INTO_INDEX('C')].previous != INTO_INDEX('E'));
        EXIT_IF(memory->vertices[INTO_INDEX('D')].distance != 1);
        EXIT_IF(memory->vertices[INTO_INDEX('D')].previous != INTO_INDEX('A'));
        EXIT_IF(memory->vertices[INTO_INDEX('E')].distance != 2);
        EXIT_IF(memory->vertices[INTO_INDEX('E')].previous != INTO_INDEX('D'));
    });
    TEST(memory, {
        SET_DISTANCE(memory, 'A', 'B', 3);
        SET_DISTANCE(memory, 'A', 'D', 4);
        SET_DISTANCE(memory, 'B', 'D', 4);
        SET_DISTANCE(memory, 'B', 'H', 1);
        SET_DISTANCE(memory, 'C', 'L', 2);
        SET_DISTANCE(memory, 'D', 'F', 5);
        SET_DISTANCE(memory, 'G', 'E', 2);
        SET_DISTANCE(memory, 'H', 'F', 3);
        SET_DISTANCE(memory, 'H', 'G', 2);
        SET_DISTANCE(memory, 'I', 'J', 6);
        SET_DISTANCE(memory, 'I', 'K', 4);
        SET_DISTANCE(memory, 'J', 'K', 4);
        SET_DISTANCE(memory, 'K', 'E', 5);
        SET_DISTANCE(memory, 'L', 'I', 4);
        SET_DISTANCE(memory, 'L', 'J', 4);
        SET_DISTANCE(memory, 'S', 'A', 7);
        SET_DISTANCE(memory, 'S', 'B', 2);
        SET_DISTANCE(memory, 'S', 'C', 3);
        u8 start = INTO_INDEX('S');
        u8 end = INTO_INDEX('E');
        dijkstra(memory, start, end);
        EXIT_IF(memory->vertices[INTO_INDEX('S')].distance != 0);
        EXIT_IF(memory->vertices[INTO_INDEX('A')].distance != 5);
        EXIT_IF(memory->vertices[INTO_INDEX('A')].previous != INTO_INDEX('B'));
        EXIT_IF(memory->vertices[INTO_INDEX('B')].distance != 2);
        EXIT_IF(memory->vertices[INTO_INDEX('B')].previous != INTO_INDEX('S'));
        EXIT_IF(memory->vertices[INTO_INDEX('F')].distance != 6);
        EXIT_IF(memory->vertices[INTO_INDEX('F')].previous != INTO_INDEX('H'));
        EXIT_IF(memory->vertices[INTO_INDEX('G')].distance != 5);
        EXIT_IF(memory->vertices[INTO_INDEX('G')].previous != INTO_INDEX('H'));
        EXIT_IF(memory->vertices[INTO_INDEX('E')].distance != 7);
        EXIT_IF(memory->vertices[INTO_INDEX('E')].previous != INTO_INDEX('G'));
    });
    fprintf(stderr, "\n");
    free(memory);
    return EXIT_SUCCESS;
}
