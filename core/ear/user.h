#pragma once

#include "texture.h"
#include "../eau/mat4.h"

#include <stdint.h>
#include <stdbool.h>

void 
ear_clear(
    float col[3]
    );

typedef enum ear_draw_mode{
    EAR_MODE_TRIANGLES,
    EAR_MODE_LINES,
} ear_draw_mode;

void
ear_draw(
    int32_t vertices, int32_t instances, ear_draw_mode draw_mode
    );

void 
ear_flush(
    void
    );


void
ear_rect(
    float x, float y,
    float w, float h,
    float col[4]
    );

void
ear_tex(
    ear_texture* tex,
    float x, float y,
    float w, float h,
    float sx, float sy, float sw, float sh,
    float col[4]
    );


void
ear_translate(
    float x, float y
    );

void
ear_scale(
    float x, float y
    );

void
ear_rotate(
    float ang
    );

void
ear_rev_translate(
    float x, float y
    );

void
ear_rev_scale(
    float x, float y
    );

void
ear_rev_rotate(
    float ang
    );

void
ear_reset_transform(
    void
    );

void
ear_save_transform(
    mat4* out
    );

void
ear_load_transform(
    mat4 mat
    );
