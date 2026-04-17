#pragma once

#include "ear/ll/framebuffer.h"
#include <stdint.h>

typedef struct ear_gl_framebuffer ear_gl_framebuffer;

extern ear_gl_framebuffer* _ear_gl_cur_fb;

void*
ear_gl_create_framebuffer(
    ear_framebuffer_desc* desc
    );
void
ear_gl_delete_framebuffer(
    void* fb
    );

void
ear_gl_bind_framebuffer(
    void* fb
    );

struct ear_gl_framebuffer{
    uint32_t id;

    bool clear;
    float r; float g; float b; float a;

    uint32_t frame;
};
