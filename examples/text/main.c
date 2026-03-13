#include "../../eat.h"

#include <stdio.h>

int main(void) {
    eat_init("text", 1600,900, (eat_init_opts){});

    static const char bm_data[] = {
    #embed "font.png"
    };

    static const char ttf_data[] = {
    #embed "roboto.ttf"
    };

    ear_font* bmp_mono_font = ear_load_bitmap_mono_font(bm_data, sizeof(bm_data), NULL);
    ear_font* truetype_font = ear_load_truetype_font(ttf_data, sizeof(ttf_data), NULL);

    while (eat_frame()) {
        ear_clear((float[3]){ .2f, .4f, .3f });

        ear_text(bmp_mono_font, "hello\n\tworld!!!!", 0,0, 8 * 4, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);
        ear_text(truetype_font, "hello\n\tworld!!!!", 0,64, 8*4, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    ear_delete_font(truetype_font);
    ear_delete_font(bmp_mono_font);

    eat_stop();

    return 0;
}
