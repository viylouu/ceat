#include "arena.h"
//#include "../cutil.h"

#include <stdlib.h>
#include <stdio.h>

#include "../ear/hl/text.h"
#include "conv.h"
#include "../ear/hl/user.h"
#include "../eau/coll.h"

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
    eat_debug_ll_obj* deb_obj,
    void (*delete)(void*)
    ) {
    *user_dest = malloc(sizeof(eau_destructor));
    **user_dest = (eau_destructor){
        .arena = arena,
        .data = data,
        .delete = delete,
        .user_dest = user_dest,
        .deb_obj = deb_obj,
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
    int32_t* selected
    ) {
    (void)w; (void)h;

    eau_arena* arena = _arena;

    float offy = 0;
    float off = 16;
    
    char buf[64];

    snprintf(buf, sizeof(buf), "items: %d", arena->dest_amt);
    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    for (uint32_t i = 0; i < arena->dest_amt; ++i) {
        eau_destructor* dest = arena->dests[i];
        if (dest->user_dest == NULL) continue;

        uint32_t idx = 0;
        for (eat_debug_ll_obj* it = eat_debug_ll_first; it != NULL; it = it->next) {
            if (it->data == dest->deb_obj->data) break;
            ++idx;
        }

        snprintf(buf, sizeof(buf), "%s %d", dest->deb_obj->type_name, idx);

        float width;
        ear_text_size(t.font, buf, 14, &width, NULL);

        bool sel = eau_point_rect(eaw_mouse_x,eaw_mouse_y, (eau_rect){ x,y+offy, width+8,16, EAU_ALIGN_TOP_LEFT });

        ear_rect(x,y+offy, width+8, 16, sel? debug_theme.sel_but_col : debug_theme.but_col, EAU_ALIGN_TOP_LEFT);
        ear_rect(x+2,y+2+offy, width+4, 12, sel? debug_theme.but_col : debug_theme.bg_col, EAU_ALIGN_TOP_LEFT);

        if (sel && eaw_is_mouse_pressed(EAW_MOUSE_LEFT)) *selected = idx;

        ear_text(t.font, buf, x + 4, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);

        offy += 18;
    }
}
