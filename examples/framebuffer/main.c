#include "../../eat.h"

#include <stdio.h>

int main(void) {
    eat_init("framebuffer", 1600,900, (eat_init_opts){});

    ear_texture* fbtex = ear_create_texture((ear_texture_desc){
            .filter = EAR_FILTER_NEAREST,
            .type = EAR_TEX_COLOR,
            .wrap = EAR_WRAP_REPEAT,
        }, NULL, 128, 128, NULL);
    ear_framebuffer* fb = ear_create_framebuffer((ear_framebuffer_desc){
            .out_colors = (ear_texture*[]){ fbtex },
            .out_color_amt = 1,
            .out_depth = NULL,
            .width = 128, .height = 128,
        }, NULL);

    static const uint8_t tex_data[] = {
    #embed "tex.png"
    };

    ear_texture* tex = ear_load_texture((ear_texture_desc){
            .filter = EAR_FILTER_NEAREST,
            .type = EAR_TEX_COLOR,
            .wrap = EAR_WRAP_REPEAT,
        }, tex_data, sizeof(tex_data), NULL);

    while (eat_frame()) {
        ear_bind_framebuffer(fb);
        ear_clear((float[3]){ .2f, .3f, .4f });
        ear_tex(tex, 0,0, 64,64, 0,0,299,291, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        ear_bind_framebuffer(NULL);
        ear_tex(fbtex, 0,eat_height, eat_width,-eat_height, 0,0,128,128, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    ear_delete_texture(tex);

    ear_delete_framebuffer(fb);
    ear_delete_texture(fbtex);

    eat_stop();

    return 0;
}
