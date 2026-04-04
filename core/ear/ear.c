#include "ear.h"
//#include "../cutil.h"

#include "hl/data.h"
#include "../eaw/window.h"
#include "ll/vk/vk.h"
#include "ll/vk/eng/screen.h"

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

    //ear_set_default_framebuffer(NULL);
    //ear_bind_framebuffer(NULL);

    ear_vk_set_viewport(0,0, _eaw_window_width, _eaw_window_height);
    ear_vk_set_scissor (0,0, _eaw_window_width, _eaw_window_height);
    eau_mat4_ortho(&proj, 0, _eaw_window_width, 0, _eaw_window_height, 0,1);
    eau_mat4_identity(&transf);
}
