#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "texture.h"
#include "user.h"

void
ear_text(
    ear_texture* atlas,
    char* text,
    float x, float y,
    float scalex, float scaley,
    float col[4],
    ear_align align
    );
