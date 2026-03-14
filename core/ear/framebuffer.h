#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../eau/arena.h"
#include "texture.h"

typedef struct ear_framebuffer ear_framebuffer;
typedef struct ear_framebuffer_desc ear_framebuffer_desc;


ear_framebuffer*
ear_create_framebuffer(
    ear_framebuffer_desc desc,
    eau_arena* arena
    );

void
ear_delete_framebuffer(
    ear_framebuffer* framebuffer
    );

void
ear_bind_framebuffer(
    ear_framebuffer* framebuffer
    );

void
ear_resize_framebuffer(
    ear_framebuffer* framebuffer,
    uint32_t width, uint32_t height
    );

void
ear_set_default_framebuffer(
    ear_framebuffer* framebuffer
    );

void
_ear_set_master_framebuffer(
    ear_framebuffer* framebuffer
    );


struct ear_framebuffer_desc{
    ear_texture** out_colors;
        uint32_t out_color_amt;
    ear_texture* out_depth;
    uint32_t width; uint32_t height;
};

struct ear_framebuffer{
    uint32_t id;

    ear_framebuffer_desc desc;

    eau_destructor* dest;
};
