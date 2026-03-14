#include "buffer.h"
#include "../cutil.h"

#include "user.h"
#include "text.h"
#include "gl.h"

GLenum
_TYPECONV_buffer_type(
    ear_buffer_type type
    );

GLenum
_TYPECONV_buffer_usage(
    ear_buffer_usage usage
    );


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
        .desc = desc,
        .data = data,
        .size = size, .prev_size = size,

        .deb_obj = eat_debug_add_obj(
            buf,
            "buffer",
            _ear_debug_buffer_window
            ),
        };

    gl.genBuffers(1, &buf->id);

    GLenum targ = _TYPECONV_buffer_type(buf->desc.type);

    gl.bindBuffer(targ, buf->id);
    gl.bufferData(
        targ,
        buf->size,
        buf->data,
        _TYPECONV_buffer_usage(buf->desc.usage)
        );
    gl.bindBuffer(targ, 0);

    if (arena != NULL) eau_add_to_arena(arena, &buf->dest, buf, _ear_arena_buffer_delete);
    return buf;
}

void
ear_delete_buffer(
    ear_buffer* buf
    ) {
    eat_debug_remove_obj(buf->deb_obj);

    gl.deleteBuffers(1, &buf->id);

    if (buf->dest != NULL) buf->dest->data = NULL;
    free(buf);
}

void
ear_bind_buffer(
    ear_buffer* buf,
    uint32_t slot
    ) {
    GLenum targ = _TYPECONV_buffer_type(buf->desc.type);

    gl.bindBuffer(targ, buf->id);

    if (buf->desc.type == EAR_BUF_UNIFORM || buf->desc.type == EAR_BUF_STORAGE)
        gl.bindBufferBase(targ, slot, buf->id);

    if (buf->desc.type == EAR_BUF_VERTEX)
        gl.bindVertexBuffer(slot, buf->id, 0, buf->desc.stride);

}

void
ear_update_buffer(
    ear_buffer* buf
    ) {
    if (buf->prev_size != 0) eat_assert(buf->desc.usage == EAR_USAGE_DYNAMIC, "cant update a static buffer!");

    GLenum targ = _TYPECONV_buffer_type(buf->desc.type);

    gl.bindBuffer(targ, buf->id);

    if (buf->size > buf->prev_size) {
        buf->prev_size = buf->size;
        gl.bufferData(targ, buf->size, buf->data, GL_DYNAMIC_DRAW);
    } else gl.bufferSubData(targ, 0, buf->size, buf->data);

    gl.bindBuffer(targ, 0);
}


void
_ear_debug_buffer_window(
    void* _buffer, 
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
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
}



GLenum
_TYPECONV_buffer_type(
    ear_buffer_type type
    ) {
    switch (type) {
    case EAR_BUF_UNIFORM: return GL_UNIFORM_BUFFER;
    case EAR_BUF_STORAGE: return GL_SHADER_STORAGE_BUFFER;
    case EAR_BUF_VERTEX:  return GL_ARRAY_BUFFER;
    }

    eat_unreachable();
}

GLenum
_TYPECONV_buffer_usage(
    ear_buffer_usage usage
    ) {
    switch (usage) {
    case EAR_USAGE_DYNAMIC: return GL_DYNAMIC_DRAW;
    case EAR_USAGE_STATIC:  return GL_STATIC_DRAW;
    }

    eat_unreachable();
}
