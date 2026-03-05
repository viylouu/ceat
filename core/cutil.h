#pragma once

#include <stdio.h>
#include <stdlib.h>

#define eat_assert(cond, ...) do { \
    if (!(cond)) { \
        printf("\x1b[1;31m [assertion failure] (\"%s\" : %d) :: ", __FILE__, __LINE__); \
        printf(__VA_ARGS__); \
        printf("\x1b[0m\n"); \
        exit(1); \
    } \
} while(0)

#define eat_unreachable() do { \
    printf("\x1b[1;32m [unreachable] (\"%s\" : %d)", __FILE__, __LINE__); \
    printf("\x1b[0m\n"); \
    exit(1); \
} while(0)
