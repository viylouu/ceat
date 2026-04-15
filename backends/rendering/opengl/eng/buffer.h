#pragma once

#include "ear/ll/buffer.h"

typedef struct ear_gl_buffer ear_gl_buffer;

extern void* _ear_gl_index_offset;

void*
ear_gl_create_buffer(
    ear_buffer_desc desc,
    const void* data,
    uint32_t size
    );
void
ear_gl_delete_buffer(
    void* buf
    );

void
ear_gl_bind_buffer(
    void* buf,
    uint32_t slot,
    uint32_t offset
    );

void
ear_gl_update_buffer(
    void* buf,
    const void* data,
    uint32_t offset
    );

struct ear_gl_buffer{
    uint32_t id;

    uint32_t chunk_bytes;
    uint32_t stride;
    uint32_t size;

    //uint32_t 

    ear_buffer_type type;
};
