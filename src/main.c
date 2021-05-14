#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CAP_LISTS 64

typedef uint8_t u8;

typedef int32_t i32;

typedef struct List List;

struct List {
    const char* label;
    List*       next;
};

typedef struct {
    List lists[CAP_LISTS];
    u8   len_lists;
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

static List* alloc_list(Memory* memory) {
    EXIT_IF(CAP_LISTS <= memory->len_lists);
    return &memory->lists[memory->len_lists++];
}

i32 main(void) {
    fprintf(stderr,
            "\n"
            "sizeof(List)   : %zu\n"
            "sizeof(Memory) : %zu\n"
            "\n",
            sizeof(List),
            sizeof(Memory));
    Memory* memory = calloc(1, sizeof(Memory));
    {
        List* a = alloc_list(memory);
        List* b = alloc_list(memory);
        List* c = alloc_list(memory);
        List* d = alloc_list(memory);
        List* e = alloc_list(memory);
        a->label = "foo";
        a->next = b;
        b->label = "bar ...";
        b->next = c;
        c->label = "\tbaz";
        c->next = a;
        d->label = "qux";
        d->next = d;
        e->label = "???";
        e->next = NULL;
    }
    {
        printf("digraph {\n");
        for (u8 i = 0; i < memory->len_lists; ++i) {
            printf("    \"%s\"", memory->lists[i].label);
            if (memory->lists[i].next) {
                printf(" -> \"%s\"", memory->lists[i].next->label);
            }
            printf("\n");
        }
        printf("}\n");
    }
    fprintf(stderr, "Done!\n");
    free(memory);
    return EXIT_SUCCESS;
}
