#include "ear.h"
//#include "../cutil.h"

#include "hl/data.h"
#include "../eaw/window.h"
#include "ll/framebuffer.h"
#include "rendering/impl.h"
//#include "ll/vk/eng/screen.h"
//#include "ll/vk/sc/swapchain.h"

bool _ear_clear_this_frame;

void
ear_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    ) {
    ear_backend->main.init(title, width, height, vsync);

    ear_user_init();
}

void 
ear_exit(
    void
    ) {
    ear_user_exit();

    ear_backend->main.exit();
}

void
ear_frame(
    bool clear
    ) {
    _ear_clear_this_frame = clear;

    ear_flush();

    ear_backend->main.frame();
    eaw_window_swapbuf();

    _ear_set_master_framebuffer(NULL);
    ear_set_default_framebuffer(NULL);
    ear_bind_framebuffer(NULL);
    
    eau_mat4_identity(&transf);
}
