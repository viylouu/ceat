#include "../eat.h"

//#include "eaw/window.h"
#include "debug/console.h"
#include "debug/debug.h"

#include "../backends/rendering/impl.h"
#include "../backends/rendering/vulkan/vk.h"

int32_t eat_width;
int32_t eat_height;

float eat_time;
float eat_delta;
    double eat_time64;
    double eat_delta64;

ear_texture* _eat_screen_color;
ear_texture* _eat_screen_depth;
ear_framebuffer* _eat_screen_framebuffer;

bool first = true;

void 
eat_init(
    const char* title,
    int32_t width, int32_t height,
    eat_init_opts opts
    ) {
    ear_backend = opts.rendering_impl;
    if (ear_backend == NULL) ear_backend = &ear_vk_impl;

    eaw_init(title, width, height);
    ear_init(title, width, height, opts.vsync);
    eaa_init();

    _eat_screen_color = ear_create_texture((ear_texture_desc){
        .type = EAR_TEX_COLOR,
        .filter = EAR_FILTER_NEAREST,
        .wrap = EAR_WRAP_REPEAT,
        }, NULL, width, height, NULL);
    _eat_screen_depth = ear_create_texture((ear_texture_desc){
        .type = EAR_TEX_DEPTH,
        .filter = EAR_FILTER_NEAREST,
        .wrap = EAR_WRAP_REPEAT,
        }, NULL, width, height, NULL);
    _eat_screen_framebuffer = ear_create_framebuffer((ear_framebuffer_desc){
        .out_colors = &_eat_screen_color,
            .out_color_amt = 1,
        .out_depth = _eat_screen_depth,
        .width = width,
        .height = height,
        }, NULL);

    debug = opts.debug;
    console = opts.console;
    if (debug.enabled || console.enabled) eat_debug_init();
    if (console.enabled) eat_console_init();
}

void
eat_exit(
    void
    ) {
    if (console.enabled) eat_console_stop();
    if (debug.enabled) eat_debug_stop();

    ear_delete_framebuffer(_eat_screen_framebuffer);
    ear_delete_texture(_eat_screen_depth);
    ear_delete_texture(_eat_screen_color);

    eaa_exit();
    ear_exit();
    eaw_exit();
}

bool
eat_frame(
    void
    ) {
    /*
    if (!first) {
        eau_tick_this_frame = false;
        if (eaw_time - _eau_last_tick > eau_tickrate) {
            _eau_last_tick = eaw_time;
            eau_tick_this_frame = true;
        }

        _ear_set_master_framebuffer(NULL);
        ear_set_default_framebuffer(NULL);
        ear_bind_framebuffer(NULL);

        if (!debug.enabled || !eat_debug_toggled) 
            ear_tex(_eat_screen_color, 0,0, eat_width,eat_height, 0,0, eat_width,-eat_height, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        if (debug.enabled) eat_debug_try_do();    
    } first = false;
    */

    //int prev_width = eat_width;
    //int prev_height = eat_height;

    ear_frame();
    eaw_frame();

    //_ear_set_master_framebuffer(_eat_screen_framebuffer);
    //ear_set_default_framebuffer(NULL);
    //ear_bind_framebuffer(NULL);

    //if (console.enabled) eat_console_try_do();
    
    eat_time = eaw_time;
    eat_delta = eaw_delta;
    eat_time64 = eaw_time;
    eat_delta64 = eaw_delta;

    eat_debug_get_screen_size(&eat_width, &eat_height);

    /*
    if (prev_width != eat_width || prev_height != eat_height) {
        ear_resize_texture(_eat_screen_color, NULL, eat_width, eat_height);
        ear_resize_texture(_eat_screen_depth, NULL, eat_width, eat_height);
        ear_resize_framebuffer(_eat_screen_framebuffer, eat_width, eat_height);
    }
    */

    return eaw_is_open();
}
