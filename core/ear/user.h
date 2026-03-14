#pragma once

#include "texture.h"
#include "../eau/mat4.h"
#include "../eau/conv.h"

#include <stdint.h>
#include <stdbool.h>

void 
ear_flush(
    void
    );

void
ear_rect(
    float x, float y,
    float w, float h,
    float col[4],
    eau_align align
    );

void
ear_tex(
    ear_texture* tex,
    float x, float y,
    float w, float h,
    float sx, float sy, float sw, float sh,
    float col[4],
    eau_align align
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
