#pragma once

#include <stdint.h>

#include "ear/ll/texture.h"

typedef struct ear_gl_texture ear_gl_texture;

void*
ear_gl_create_texture(
    ear_texture_desc desc,
    uint8_t** pixels,
    uint32_t width, uint32_t height
    );
void
ear_gl_delete_texture(
    void* tex
    );

void
ear_gl_update_texture(
    void* tex,
    const uint8_t* pixels,
    uint32_t width, uint32_t height
    );

struct ear_gl_texture{
    uint32_t id;

    uint32_t width; uint32_t height;

    ear_texture_desc desc;
};
