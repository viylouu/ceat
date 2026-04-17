#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../debug/debug.h"

typedef struct eau_arena eau_arena;

typedef struct eau_destructor eau_destructor;

eau_arena*
eau_create_arena(
    void
    );

void
eau_delete_arena(
    eau_arena* arena
    );

void
eau_add_to_arena(
    eau_arena* arena,
    eau_destructor** user_dest,
    void* data,
    eat_debug_ll_obj* deb_obj,
#ifdef __cplusplus
    void (*delete_fn)(void* data)
#else
    void (*delete)(void* data)
#endif
    );

void
eau_clear_arena(
    eau_arena* arena
    );


struct eau_destructor{
    eau_arena* arena;
    void* data;
    eat_debug_ll_obj* deb_obj;
#ifdef __cplusplus
    void (*delete_fn)(void*);
#else
    void (*delete)(void*);
#endif
    eau_destructor** user_dest;
};

struct eau_arena{
    eau_destructor** dests;
    uint32_t dest_amt;

    eat_debug_ll_obj* deb_obj;
};
