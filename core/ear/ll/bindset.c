#include "bindset.h"
#include "../../cutil.h"

#include <stdlib.h>
#include <string.h>

#include "rendering/impl.h"

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
        .vk = ear_backend->bindset.create(desc),

        .desc = desc,

        .deb_obj = eat_debug_add_obj(
            set,
            "bindset",
            _ear_debug_bindset_window,
            arena
            ),
        };

    for (uint32_t i = 0; i < desc.binding_amt; ++i)
        if (desc.bindings[i].type != EAR_BIND_TEXTURE2D) ++set->offsettable;

    if (arena != NULL) eau_add_to_arena(arena, &set->dest, set, _ear_arena_bindset_delete);
    return set;
}
void
ear_delete_bindset(
    ear_bindset* set
    ) {
    eat_debug_remove_obj(set->deb_obj);

    ear_backend->bindset.delete(set->vk);

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
    if (offset_amt != 0) eat_assert(offset_amt == set->offsettable, "offset amt must be amount of offsettable bindings!");

    uint32_t offamt = offset_amt > 0? offset_amt : set->offsettable;
    uint32_t offs[offamt == 0? 1 : offamt];
    if (offamt > 0) {
        uint32_t bind_i = 0;
        for (uint32_t i = 0; i < offamt; ++i) {
            while (bind_i < set->desc.binding_amt &&
                   set->desc.bindings[bind_i].type == EAR_BIND_TEXTURE2D) 
                ++bind_i;
            eat_assert(bind_i < set->desc.binding_amt, "more offsets sent than there are offsettable bindings!");

            ear_buffer* buf = set->desc.bindings[bind_i].object; 
            eat_assert(buf != NULL, "binding buffer cannot be null, especially on binding!");

            offs[i] = offset_amt == 0? 0 : (offsets[i] * buf->desc.chunk_size * buf->desc.stride);

            ++bind_i;
        }
    }

    ear_backend->bindset.bind(set->vk, slot, offs, offamt);
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
