#ifndef __PRELUDE_H__
#define __PRELUDE_H__

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STATIC_ASSERT _Static_assert

typedef uint8_t  u8;
typedef uint64_t u64;
typedef int32_t  i32;

#define MAX_U8 0xFF

typedef enum {
    FALSE = 0,
    TRUE,
} Bool;

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

#endif
