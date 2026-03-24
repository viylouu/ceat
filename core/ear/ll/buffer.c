#include "buffer.h"
#include "../../cutil.h"

#include "vk/eng/buffer.h"

#include "../hl/user.h"
#include "../hl/text.h"

void
_ear_arena_buffer_delete(
    void* buf
    ) { 
    ear_delete_buffer(buf); 
}

void
_ear_debug_buffer_window(
    void* buffer, 
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

ear_buffer*
ear_create_buffer(
    ear_buffer_desc desc,
    void* data,
    uint32_t size,
    eau_arena* arena
    ) {
    ear_buffer* buf = malloc(sizeof(ear_buffer));
    *buf = (ear_buffer){
        .vk = ear_vk_create_buffer(desc, data, size),

        .desc = desc,
        .data = data,
        .size = size, .prev_size = size,

        .deb_obj = eat_debug_add_obj(
            buf,
            "buffer",
            _ear_debug_buffer_window,
            arena
            ),
        };

    

    if (arena != NULL) eau_add_to_arena(arena, &buf->dest, buf, _ear_arena_buffer_delete);
    return buf;
}

void
ear_delete_buffer(
    ear_buffer* buf
    ) {
    eat_debug_remove_obj(buf->deb_obj);

    ear_vk_delete_buffer(buf->vk);

    if (buf->dest != NULL) buf->dest->data = NULL;
    free(buf);
}

void
ear_bind_buffer(
    ear_buffer* buf,
    uint32_t slot
    ) {
    ear_vk_bind_buffer(buf->vk, slot);
}

void
ear_update_buffer(
    ear_buffer* buf
    ) {
    /*
    if (buf->prev_size != 0) eat_assert(buf->desc.usage == EAR_USAGE_DYNAMIC, "cant update a static buffer!");

    GLenum targ = _TYPECONV_buffer_type(buf->desc.type);

    gl.bindBuffer(targ, buf->id);

    if (buf->size > buf->prev_size) {
        buf->prev_size = buf->size;
        gl.bufferData(targ, buf->size, buf->data, GL_DYNAMIC_DRAW);
    } else gl.bufferSubData(targ, 0, buf->size, buf->data);

    gl.bindBuffer(targ, 0);
    */
}


void
_ear_debug_buffer_window(
    void* _buffer, 
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
    /*
    ear_buffer* buffer = _buffer;

    float offy = 0;
    float off = 16;
    
    char buf[64];

    switch (buffer->desc.type) {
    case EAR_BUF_VERTEX: snprintf(buf, sizeof(buf), "type: vertex"); break;
    case EAR_BUF_UNIFORM: snprintf(buf, sizeof(buf), "type: uniform"); break;
    case EAR_BUF_STORAGE: snprintf(buf, sizeof(buf), "type: storage"); break;
    }

    ear_text(t.font, buf, x, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    switch (buffer->desc.usage) {
    case EAR_USAGE_DYNAMIC: snprintf(buf, sizeof(buf), "usage: dynamic");
    case EAR_USAGE_STATIC: snprintf(buf, sizeof(buf), "usage: static");
    }

    ear_text(t.font, buf, x, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    snprintf(buf, sizeof(buf), "elements: %d", buffer->size / buffer->desc.stride);
    ear_text(t.font, buf, x, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    snprintf(buf, sizeof(buf), "stride: %d", buffer->desc.stride);
    ear_text(t.font, buf, x, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;
    */
}
