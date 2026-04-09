#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "framebuffer.h"

void
ear_clear_color(
    ear_framebuffer* fb,
    float r, float g, float b, float a
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
