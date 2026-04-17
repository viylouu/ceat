#include "bindset.h"
#include "cutil.h"

#include "../init/funcs.h"
#include "../util/macros.h"
#include "ear/ll/texture.h"
#include "texture.h"
#include "../util/buffer.h"
#include "buffer.h"

#include <stdlib.h>

void*
ear_gl_create_bindset(
    ear_bindset_desc desc
    ) {
    ear_gl_bindset* set = malloc(sizeof(ear_gl_bindset));
    set->desc = desc;

    return set;
}
void
ear_gl_delete_bindset(
    void* set
    ) {
    free(set);
}

void
ear_gl_update_bindset(
    void* _set,
    ear_bindset_desc desc
    ) {
    ear_gl_bindset* set = _set;
    set->desc = desc;
}

void
ear_gl_bind_bindset(
    void* _set,
    uint32_t slot,
    const uint32_t* offsets,
    uint32_t offset_amt
    ) {
    (void)slot;
    (void)offset_amt;

    ear_gl_bindset* set = _set;

    uint32_t bind_i = 0;
    for (uint32_t i = 0; i < set->desc.binding_amt; ++i) {
        ear_bind_desc bind = set->desc.bindings[i];

        eat_assert(bind.object != NULL, "bind object cannot be null!");

        uint32_t off = 0;
        if (set->desc.bindings[i].type != EAR_BIND_TEXTURE2D) { 
            off = offsets[bind_i];
            ++bind_i;

            ear_buffer* buf = bind.object;
            ear_gl_buffer* gbuf = get_gl(buffer, buf);

            switch(buf->desc.type) {
            case EAR_BUF_STORAGE:
            case EAR_BUF_UNIFORM:
                gl.bindBufferRange(_ear_gl_convert_buffer_type(buf->desc.type), bind.binding, gbuf->id, off * gbuf->chunk_bytes, gbuf->chunk_bytes);
                break;
            case EAR_BUF_VERTEX:
            case EAR_BUF_INDEX:
                eat_error("vertex/index buffers cannot be in a bindset!");
            }
        } else {
            gl.activeTexture(GL_TEXTURE0 + bind.binding);
            gl.bindTexture(GL_TEXTURE_2D, get_gl(texture, (ear_texture*)bind.object)->id);
            gl.uniform1i(bind.binding, bind.binding);
        }
    }
}
