#include "ear.h"
//#include "../cutil.h"

//#include "data.h"
#include "ll/vk/vk.h"

void
ear_init(
    void
    ) {
    //ear_gl_init();
    //ear_user_init();

    //gl.enable(GL_SCISSOR_TEST);
    
    ear_vk_init();
}

void 
ear_stop(
    void
    ) {
    ear_vk_exit();

    //ear_user_stop();
}

void
ear_frame(
    void
    ) {
    //ear_set_default_framebuffer(NULL);
    //ear_bind_framebuffer(NULL);

    //eau_mat4_identity(&transf);
}
