#pragma once

#include "../impl.h"
#include "../../../eat.h"

extern eat_backend_rendering_impl ear_gl_impl;
extern eat_plugin ear_gl_plugin;

extern uint32_t _ear_gl_frame;
extern int32_t _ear_gl_uni_align;

void
ear_gl_init(
    void
    );
void
ear_gl_exit(
    void
    );

void
ear_gl_frame(
    void
    );
