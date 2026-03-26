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

#define eat_error(...) do { \
    printf("\x1b[1;31m [error] (\"%s\" : %d) :: ", __FILE__, __LINE__); \
    printf(__VA_ARGS__); \
    printf("\x1b[0m\n"); \
    exit(1); \
} while(0)

#define eat_warn(...) do { \
    printf("\x1b[1;32m [warning] (\"%s\" : %d) :: ", __FILE__, __LINE__); \
    printf(__VA_ARGS__); \
    printf("\x1b[0m\n"); \
} while(0)

#define eat_unreachable() do { \
    printf("\x1b[1;32m [unreachable] (\"%s\" : %d)", __FILE__, __LINE__); \
    printf("\x1b[0m\n"); \
    exit(1); \
} while(0)

#define EAR_DEBUG
#ifdef EAR_DEBUG
#define eat_debug(...) do { \
    printf("\x1b[1;33m [debug] (\"%s\" : %d)", __FILE__, __LINE__); \
    printf(__VA_ARGS__); \
    printf("\x1b[0m\n"); \
} while(0)
#else
#define eat_debug(...)
#endif
