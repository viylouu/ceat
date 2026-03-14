#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "core/eaw/eaw.h"
#include "core/ear/ear.h"
#include "core/eau/eau.h"
#include "core/eaa/eaa.h"

#include "core/debug/debug.h"
#include "core/debug/console.h"

#include <stdint.h>
#include <stdbool.h>

extern int32_t eat_width;
extern int32_t eat_height;

extern float eat_time;
extern float eat_delta;
    extern double eat_time64;
    extern double eat_delta64;

extern ear_texture* _eat_screen_color;
extern ear_texture* _eat_screen_depth;
extern ear_framebuffer* _eat_screen_framebuffer;




typedef struct eat_init_opts {
    bool vsync;
    eat_console_desc console;
    eat_debug_desc debug;
} eat_init_opts;

void 
eat_init(
    const char* title, 
    int32_t width, int32_t height,
    eat_init_opts opts
    );

void
eat_stop(
    void
    );

bool
eat_frame(
    void
    );

#ifdef __cplusplus
}
#endif
