#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum eat_plugin_type{
    EAT_PLUG_BACKEND_WINDOW,
    EAT_PLUG_BACKEND_RENDER,
    EAT_PLUG_GENERIC,
} eat_plugin_type;

typedef struct eat_plugin{
    eat_plugin_type type;
    const char* name;

    // only for non-generic plugins
    // used for selection between plugins of the same type
    int32_t (*score)(void);

    void (*init)(void);
    void (*exit)(void);
} eat_plugin;

extern int32_t eat_width;
extern int32_t eat_height;

extern float eat_time;
extern float eat_delta;
    extern double eat_time64;
    extern double eat_delta64;

void
eat_run(
    const char* title,
    uint32_t width, uint32_t height,
    bool vsync,

    const eat_plugin* plugs[],
    uint32_t plug_amt
    );

/*

#pragma once

#include "backends/rendering/impl.h"

#include "core/eaw/eaw.h"
#include "core/ear/ear.h"
#include "core/eau/eau.h"
#include "core/eaa/eaa.h"

#include "core/debug/debug.h"
#include "core/debug/console.h"

#include <stdint.h>
#include <stdbool.h>

extern ear_texture* _eat_screen_color;
extern ear_texture* _eat_screen_depth;
extern ear_framebuffer* _eat_screen_framebuffer;

typedef struct eat_init_opts {
    bool vsync;
    eat_console_desc console;
    eat_debug_desc debug;

    eat_backend_rendering_impl* rendering_impl;
} eat_init_opts;

void 
eat_init(
    const char* title, 
    int32_t width, int32_t height,
    eat_init_opts opts
    );

void
eat_exit(
    void
    );

bool
eat_frame(
    void
    );

*/
