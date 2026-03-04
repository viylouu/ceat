#include "../../eat.h"

#include <stdio.h>

int main(void) {
    eat_init("text", 1600,900, (eat_init_opts){});

    static const uint8_t tex_data[] = {
    #embed "font.png"
    };

    ear_texture* font = ear_load_texture((ear_texture_desc){
            .filter = EAR_FILTER_NEAREST,
            .type = EAR_TEX_COLOR,
            .wrap = EAR_WRAP_REPEAT,
        }, tex_data, sizeof(tex_data));

    while (eat_frame()) {
        ear_clear((float[3]){ .2f, .4f, .3f });

        ear_text(font, "hello\n\tworld!!!!", 0,0, 16,16, (float[4]){ 1,1,1,1 });

        printf("%.3f FPS\n", 1.f/eat_delta);
    }

    ear_delete_texture(font);

    eat_stop();

    return 0;
}
