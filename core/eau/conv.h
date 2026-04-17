#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct eau_rect eau_rect;

typedef enum eau_align{
    EAU_ALIGN_TOP_LEFT, EAU_ALIGN_TOP, EAU_ALIGN_TOP_RIGHT,
    EAU_ALIGN_MID_LEFT, EAU_ALIGN_MID, EAU_ALIGN_MID_RIGHT,
    EAU_ALIGN_BOT_LEFT, EAU_ALIGN_BOT, EAU_ALIGN_BOT_RIGHT,
} eau_align;

eau_rect
eau_rect_topleftify(
    eau_rect rect
    );

eau_rect
eau_rect_alignify(
    eau_rect rect,
    eau_align align
    );

eau_rect
eau_rect_scale_to_fit(
    eau_rect rect,
    eau_rect fit
    );

eau_rect
eau_rect_space_convert(
    eau_rect rect,
    eau_rect original_space,
    eau_rect new_space
    );

void
eau_point_space_convert(
    float x, float y,
    eau_rect original_space,
    eau_rect new_space,
    float* outx, float* outy
    );


struct eau_rect{
    float x; float y;
    float w; float h;
    eau_align align;
};
