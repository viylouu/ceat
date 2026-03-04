#pragma once

#include <stdint.h>
#include <stdbool.h>

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

eau_destructor*
eau_add_to_arena(
    eau_arena* arena,
    eau_destructor** user_dest,
    void* data,
    void (*delete)(void* data)
    );

void
eau_clear_arena(
    eau_arena* arena
    );


struct eau_destructor{
    eau_arena* arena;
    void* data;
    void (*delete)(void*);
    eau_destructor** user_dest;
};

struct eau_arena{
    eau_destructor** dests;
    uint32_t dest_amt;
};
