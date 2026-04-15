#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../../eau/arena.h"
#include "../../debug/debug.h"

#include "buffer.h"

typedef struct ear_bindset ear_bindset;
typedef struct ear_bindset_desc ear_bindset_desc;
typedef struct ear_bind_desc ear_bind_desc;

typedef enum ear_bind_type{
    EAR_BIND_UNIFORM,
    EAR_BIND_STORAGE,
    EAR_BIND_TEXTURE2D,
} ear_bind_type;

ear_bindset*
ear_create_bindset(
    ear_bindset_desc desc,
    eau_arena* arena
    );
void
ear_delete_bindset(
    ear_bindset* set
    );

void
ear_bind_bindset(
    ear_bindset* set,
    uint32_t slot,
    uint32_t offsets[],
    uint32_t offset_amt 
    );

struct ear_bindset_desc{
    ear_bind_desc* bindings;
        uint32_t binding_amt;
};

struct ear_bind_desc{
    void* object;
    ear_bind_type type;
    uint32_t binding;
    ear_shader_stage stage;
};

struct ear_bindset{
    void* vk;

    uint32_t offsettable;

    ear_bindset_desc desc;

    eau_destructor* dest;
    eat_debug_ll_obj* deb_obj;
};
