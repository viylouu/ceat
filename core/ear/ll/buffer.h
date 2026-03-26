#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../../eau/arena.h"
#include "../../debug/debug.h"

#include "pipeline.h"

typedef struct ear_buffer ear_buffer;
typedef struct ear_buffer_desc ear_buffer_desc;

typedef enum ear_buffer_type{
    EAR_BUF_UNIFORM,
    EAR_BUF_STORAGE,
    EAR_BUF_VERTEX,
    EAR_BUF_INDEX,
} ear_buffer_type;

typedef enum ear_buffer_usage{
    EAR_USAGE_DYNAMIC,
    EAR_USAGE_STATIC,
} ear_buffer_usage;

ear_buffer*
ear_create_buffer(
    ear_buffer_desc desc,
    void* data,
    uint32_t size,
    eau_arena* arena
    );

void
ear_delete_buffer(
    ear_buffer* buffer
    );

void
ear_bind_buffer(
    ear_buffer* buffer,
    uint32_t slot
    );

void
ear_update_buffer(
    ear_buffer* buffer
    );


struct ear_buffer_desc{
    ear_buffer_type type;
    //ear_buffer_usage usage;
    uint32_t stride;

    ear_shader_stage stage;
    uint32_t binding;
};

struct ear_buffer{
    void* vk;

    uint32_t size;
        uint32_t prev_size;
    void* data;

    ear_buffer_desc desc;

    eau_destructor* dest;
    eat_debug_ll_obj* deb_obj;
};
