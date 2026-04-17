#pragma once

#include <stdint.h>

void
_ear_gl_clear(
    void
    );

void
ear_gl_clear_color(
    void* _fb,
    float r, float g, float b, float a
    );

void
ear_gl_set_viewport(
    float x, float y,
    float w, float h
    );
void
ear_gl_set_scissor(
    float x, float y,
    float w, float h
    );

void
ear_gl_draw(
    uint32_t vertices, uint32_t instances
    );
void
ear_gl_draw_idx(
    uint32_t indices, uint32_t instances
    );
