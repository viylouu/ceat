#include "buffer.h"
#include "cutil.h"

#include "../init/funcs.h" 
#include "../util/buffer.h"
#include "ear/ll/buffer.h"
#include "../gl.h"
#include "../util/macros.h"

void* _ear_gl_index_offset = 0;

void*
ear_gl_create_buffer(
    ear_buffer_desc desc,
    const void* data,
    uint32_t size
    ) {
    ear_gl_buffer* buf = malloc(sizeof(ear_gl_buffer));
    buf->size = size;
    buf->type = desc.type;
    buf->stride = desc.stride;
    buf->chunk_bytes = desc.chunk_size > 0?
        desc.chunk_size * desc.stride :
        size;

    if (desc.type == EAR_BUF_UNIFORM) {
        uint32_t chunks = size / buf->chunk_bytes;
        buf->chunk_bytes = _alignup(buf->chunk_bytes, _ear_gl_uni_align);
        buf->size = buf->chunk_bytes * chunks;
    }

    gl.genBuffers(1, &buf->id);

    GLenum targ = _ear_gl_convert_buffer_type(desc.type);

    gl.bindBuffer(targ, buf->id);
    gl.bufferData(
        targ,
        buf->size,
        NULL,
        GL_DYNAMIC_DRAW
        );

    for (uint32_t i = 0; i < buf->size/buf->chunk_bytes; ++i) {
        gl.bufferSubData(
            targ,
            i * buf->chunk_bytes,
            buf->chunk_bytes,
            data
            );
    }

    return buf;
}
void
ear_gl_delete_buffer(
    void* _buf
    ) {
    ear_gl_buffer* buf = _buf;

    gl.deleteBuffers(1, &buf->id);

    free(buf);
}

void
ear_gl_bind_buffer(
    void* _buf,
    uint32_t slot,
    uint32_t offset
    ) {
    ear_gl_buffer* buf = _buf;

    switch (buf->type) {
    case EAR_BUF_VERTEX:
        gl.bindVertexBuffer(slot, buf->id, offset * buf->chunk_bytes, buf->stride);
        break;
    case EAR_BUF_INDEX:
        gl.bindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->id);
        _ear_gl_index_offset = (void*)((uint64_t)offset * buf->chunk_bytes);
        break;
    case EAR_BUF_STORAGE:
    case EAR_BUF_UNIFORM:
        eat_error("only vertex and index buffers can be directly bound! use bindsets for other buffer types!");
    default: eat_unreachable();
    }
}

void
ear_gl_update_buffer(
    void* _buf,
    const void* data,
    uint32_t offset
    ) {
    ear_gl_buffer* buf = _buf;

    GLenum targ = _ear_gl_convert_buffer_type(buf->type);

    gl.bindBuffer(targ, buf->id);

    gl.bufferSubData(targ, offset * buf->chunk_bytes, buf->chunk_bytes, data);
}
