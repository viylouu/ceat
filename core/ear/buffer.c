#include "buffer.h"
#include "../cutil.h"

#include "gl.h"

#include <GL/gl.h>
#include <GL/glext.h>

GLenum
_TYPECONV_buffer_type(
    ear_buffer_type type
    );

GLenum
_TYPECONV_buffer_usage(
    ear_buffer_usage usage
    );


ear_buffer*
ear_create_buffer(
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    ) {
    ear_buffer* buf = malloc(sizeof(ear_buffer));
    *buf = (ear_buffer){
        .desc = desc,
        .data = data,
        .size = size, .prev_size = size,
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

    return buf;
}

void
ear_delete_buffer(
    ear_buffer* buf
    ) {
    gl.deleteBuffers(1, &buf->id);

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
