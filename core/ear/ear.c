#include "ear.h"
//#include "../cutil.h"

#include "hl/data.h"
//#include "../eaw/window.h"
#include "ll/framebuffer.h"
#include "ll/vk/vk.h"
//#include "ll/vk/eng/screen.h"
//#include "ll/vk/sc/swapchain.h"

void
ear_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    ) {
    ear_vk_init(title, width, height, vsync);

    ear_user_init();
}

void 
ear_exit(
    void
    ) {
    ear_user_exit();

    ear_vk_exit();
}

void
ear_frame(
    void
    ) {
    ear_flush();

    ear_vk_frame();

    _ear_set_master_framebuffer(NULL);
    ear_set_default_framebuffer(NULL);
    ear_bind_framebuffer(NULL);
    
    eau_mat4_identity(&transf);

    //ear_set_default_framebuffer(NULL);
    //ear_bind_framebuffer(NULL);
}
