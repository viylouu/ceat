#include "ear.h"
//#include "../cutil.h"

//#include "data.h"
#include "ll/vk/vk.h"

void
ear_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    ) {
    //ear_gl_init();
    //ear_user_init();

    //gl.enable(GL_SCISSOR_TEST);
    
    ear_vk_init(title, width, height, vsync);
}

void 
ear_exit(
    void
    ) {
    ear_vk_exit();

    //ear_user_stop();
}

void
ear_frame(
    void
    ) {
    ear_vk_frame();

    //ear_set_default_framebuffer(NULL);
    //ear_bind_framebuffer(NULL);

    //eau_mat4_identity(&transf);
}
