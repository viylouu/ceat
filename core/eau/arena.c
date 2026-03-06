#include "arena.h"
#include "../cutil.h"

eau_arena*
eau_create_arena(
    void
    ) {
    eau_arena* arena = malloc(sizeof(eau_arena));
    *arena = (eau_arena){
        .dests = malloc(1),
        .dest_amt = 0,
        };

    return arena;
}

void
eau_delete_arena(
    eau_arena* arena
    ) {
    eau_clear_arena(arena);
    free(arena->dests);

    free(arena);
}

void
eau_add_to_arena(
    eau_arena* arena,
    eau_destructor** user_dest,
    void* data,
    void (*delete)(void*)
    ) {
    *user_dest = malloc(sizeof(eau_destructor));
    **user_dest = (eau_destructor){
        .arena = arena,
        .data = data,
        .delete = delete,
        .user_dest = user_dest,
        };

    ++arena->dest_amt;
    arena->dests = realloc(arena->dests, sizeof(eau_destructor) * arena->dest_amt);
    arena->dests[arena->dest_amt-1] = *user_dest;
}

void
eau_clear_arena(
    eau_arena* arena
    ) {
    for (uint32_t i = 0; i < arena->dest_amt; ++i) {
        eau_destructor* dest = arena->dests[i];
        if (dest->data != NULL) {
            dest->delete(dest->data);
            *dest->user_dest = NULL;
        }
        free(dest);
    }
    arena->dest_amt = 0;
}
