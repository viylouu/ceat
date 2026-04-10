#include "../../eat.h"

#include <stdio.h>

int main(void) {
    eat_init("clock", 1600,900, (eat_init_opts){});

    static const char tex_data[] = {
    #embed "font.png"
    };

    ear_font* font = ear_load_bitmap_mono_font(tex_data, sizeof(tex_data), NULL);

    eau_clock* clock = eau_create_clock(false, NULL);

    bool paused = true;
    float speed = 1;

    ear_clear_color(NULL, .2f, .4f, .3f, 1);
    while (eat_frame()) {
        eau_update_clocks();

        char buf[64];
        snprintf(buf, sizeof(buf), "%.3f", clock->time);

        ear_text(font, buf, 0,0, 8 * 4, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        ear_text(font, "space to start/stop\nr to reset\nleft/right to decrease/increase speed by 1", 0,64, 8 * 4, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        if (eaw_is_key_pressed(EAW_KEY_SPACE)) {
            paused = !paused;
            if (!paused) eau_start_clock(clock);
            else eau_stop_clock(clock);
        }

        if (eaw_is_key_pressed(EAW_KEY_R)) {
            eau_reset_clock(clock);
        }

        if (eaw_is_key_pressed(EAW_KEY_LEFT)) {
            speed -= 1;
            eau_set_clock_speed(clock, speed);
        } else if(eaw_is_key_pressed(EAW_KEY_RIGHT)) {
            speed += 1;
            eau_set_clock_speed(clock, speed);
        }
    }

    eau_delete_clock(clock);

    ear_delete_font(font);

    eat_exit();

    return 0;
}

