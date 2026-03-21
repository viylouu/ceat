#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum ear_draw_mode{
    EAR_MODE_TRIANGLES,
    EAR_MODE_LINES,
} ear_draw_mode;

void 
ear_clear(
    float col[3]
    );

void
ear_draw(
    int32_t vertices, int32_t instances, ear_draw_mode draw_mode
    );

void
ear_mask(
    float x, float y, float w, float h
    );
