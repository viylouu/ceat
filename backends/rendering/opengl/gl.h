#pragma once

#include "../impl.h"

extern eat_backend_rendering_impl ear_gl_impl;

extern uint32_t _ear_gl_frame;

void
ear_gl_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    );
void
ear_gl_exit(
    void
    );

void
ear_gl_frame(
    void
    );
