#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../eau/arena.h"
#include "texture.h"

typedef struct ear_texarray ear_texarray;
typedef struct ear_texarray_desc ear_texarray_desc;


ear_texarray*
ear_create_texarray(
    ear_texarray_desc desc,
    eau_arena* arena
    );

void
ear_delete_texarray(
    ear_texarray* texarray
    );

void
ear_bind_texarray(
    ear_texarray* texarray,
    uint32_t slot
    );

void
ear_add_to_texarray(
    ear_texarray* texarray,
    ear_texture* tex,
    uint32_t layer
    );

void
ear_update_texarray(
    ear_texarray* texarray
    );

void
ear_update_texarray_layer(
    ear_texarray* texarray,
    uint32_t layer
    );


struct ear_texarray_desc{
    ear_texture_filter filter;
    ear_texture_type type;
    ear_texture_wrap wrap;
        float wrap_color[4];

    uint32_t width; uint32_t height;
    uint32_t layers;
};

struct ear_texarray{
    uint32_t id;

    ear_texture** texs;

    ear_texarray_desc desc;

    eau_destructor* dest;
};
