#include "../../eat.h"

#include "../../backends/rendering/opengl/gl.h"

#include <stdio.h>

int main(void) {
    eat_init("texture", 1600,900, (eat_init_opts){ .rendering_impl = &ear_gl_impl });

    static const char tex_data[] = {
        #embed "tex.png"
        };

    ear_texture* tex = ear_load_texture((ear_texture_desc){
        .filter = EAR_FILTER_LINEAR,
        .type = EAR_TEX_COLOR,
        .wrap = EAR_WRAP_REPEAT,
        }, tex_data, sizeof(tex_data), NULL);

    ear_clear_color(NULL, .2f, .4f, .3f, 1);
    while (eat_frame()) {
        ear_tex(tex, 0,0, 512,512, 0,0,299,291, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    ear_delete_texture(tex);

    eat_exit();

    return 0;
}
