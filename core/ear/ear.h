#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "ll/pipeline.h"
#include "ll/misc.h"
#include "ll/buffer.h"
#include "ll/bindset.h"
#include "ll/texture.h"
#include "ll/framebuffer.h"
//#include "ll/texarray.h"

#include "hl/user.h"
#include "hl/text.h"
#include "hl/camera.h"

extern bool _ear_clear_this_frame;

void
ear_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    );

void 
ear_exit(
    void
    );

void
ear_frame(
    bool clear
    );
