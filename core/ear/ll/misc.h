#pragma once

#include <stdint.h>
#include <stdbool.h>

void
ear_draw(
    int32_t vertices, int32_t instances
    );

void
ear_mask(
    float x, float y, float w, float h
    );
