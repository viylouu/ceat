#include "bindset.h"
//#include "../../cutil.h"

#include <stdlib.h>
#include <string.h>

#include "vk/eng/bindset.h"

void
_ear_arena_bindset_delete(
    void* set
    ) { 
    ear_delete_bindset(set); 
}

void
_ear_debug_bindset_window(
    void* bindset,
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

ear_bindset*
ear_create_bindset(
    ear_bindset_desc u_desc,
    eau_arena* arena
    ) {
    ear_bindset_desc desc = u_desc;
    desc.bindings = malloc(sizeof(ear_bind_desc) * desc.binding_amt);
    memcpy(desc.bindings, u_desc.bindings, sizeof(ear_bind_desc) * desc.binding_amt);

    ear_bindset* set = malloc(sizeof(ear_bindset));
    *set = (ear_bindset){
        .vk = ear_vk_create_bindset(desc),

        .desc = desc,

        .deb_obj = eat_debug_add_obj(
            set,
            "bindset",
            _ear_debug_bindset_window,
            arena
            ),
        };

    if (arena != NULL) eau_add_to_arena(arena, &set->dest, set, _ear_arena_bindset_delete);
    return set;
}
void
ear_delete_bindset(
    ear_bindset* set
    ) {
    eat_debug_remove_obj(set->deb_obj);

    ear_vk_delete_bindset(set->vk);

    free(set->desc.bindings);

    if (set->dest != NULL) set->dest->data = NULL;
    free(set);
}

void
ear_bind_bindset(
    ear_bindset* set,
    uint32_t slot,
    uint32_t offsets[],
    uint32_t offset_amt
    ) {
    ear_vk_bind_bindset(set->vk, slot, offsets, offset_amt);
}


void
_ear_debug_bindset_window(
    void* _bindset,
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
    (void)_bindset;
    (void)x; (void)y; (void)w; (void)h;
    (void)t; (void)sel;
}
