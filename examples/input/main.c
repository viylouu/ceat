#include "../../eat.h"

#include "../../backends/rendering/opengl/gl.h"

#include <stdio.h>

int main(void) {
    eat_init("input", 1600,900, (eat_init_opts){ .rendering_impl = &ear_gl_impl });

    float x = 0; float y = 0;

    ear_clear_color(NULL, .2f, .4f, .3f, 1);
    while (eat_frame()) {
        const float speed = 256;

        if (eaw_is_key(EAW_KEY_LEFT)) x -= eat_delta * speed;
        if (eaw_is_key(EAW_KEY_RIGHT)) x += eat_delta * speed;
        if (eaw_is_key(EAW_KEY_UP)) y -= eat_delta * speed;
        if (eaw_is_key(EAW_KEY_DOWN)) y += eat_delta * speed;

        ear_rect(0,0, 512,512, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        ear_rect(x,y, 64,64, eaw_is_mouse(EAW_MOUSE_LEFT)? (float[4]){ 0,1,0,1 } : (float[4]){ 1,0,0,1 }, EAU_ALIGN_TOP_LEFT);

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    eat_exit();

    return 0;
}
