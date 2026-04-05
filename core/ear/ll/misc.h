#pragma once

#include <stdint.h>
#include <stdbool.h>

extern float _ear_clear_color[3];

// use when MASTER framebuffer is null
// - not default in user render pass
void
ear_clear_color(
    float r, float g, float b
    );
// use basically all the time
void
ear_clear(
    float r, float g, float b
    );

void
ear_draw(
    uint32_t vertices, uint32_t instances
    );
void
ear_draw_idx(
    uint32_t indices, uint32_t instances
    );

void
ear_mask(
    float x, float y, float w, float h
    );
