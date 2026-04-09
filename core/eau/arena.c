#include "arena.h"
//#include "../cutil.h"

#include <stdlib.h>

//#include "../ear/text.h"
//#include "conv.h"

void
_eau_debug_arena_window(
    void* arena,
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

eau_arena*
eau_create_arena(
    void
    ) {
    eau_arena* arena = malloc(sizeof(eau_arena));
    *arena = (eau_arena){
        .dests = malloc(1),
        .dest_amt = 0,

        .deb_obj = eat_debug_add_obj(
            arena,
            "arena",
            _eau_debug_arena_window,
            NULL
            ),
        };

    return arena;
}

void
eau_delete_arena(
    eau_arena* arena
    ) {
    eat_debug_remove_obj(arena->deb_obj);

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
        if (dest->data != NULL) dest->delete(dest->data);
        free(dest);
    }
    arena->dest_amt = 0;
}


void
_eau_debug_arena_window(
    void* _arena,
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
    (void)_arena;
    (void)x; (void)y; (void)w; (void)h;
    (void)t; (void)sel;

    /*
    eau_arena* arena = _arena;

    float offy = 0;
    float off = 16;
    
    char buf[64];

    snprintf(buf, sizeof(buf), "items: %d", arena->dest_amt);
    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;
    */
}
