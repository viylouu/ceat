#include "../../eat.h"

#include <stdio.h>

int main(void) {
    eat_init("clock", 1600,900, (eat_init_opts){});

    static const uint8_t tex_data[] = {
    #embed "font.png"
    };

    ear_texture* font = ear_load_texture((ear_texture_desc){
            .filter = EAR_FILTER_NEAREST,
            .type = EAR_TEX_COLOR,
            .wrap = EAR_WRAP_REPEAT,
        }, tex_data, sizeof(tex_data), NULL);

    eau_clock* clock = eau_create_clock(NULL);

    bool paused = true;
    float speed = 1;

    while (eat_frame()) {
        ear_clear((float[3]){ .2f, .4f, .3f });

        eau_update_clocks();

        char buf[64];
        snprintf(buf, sizeof(buf), "%.3f", clock->time);

        ear_text(font, buf, 0,0, 4,4, (float[4]){ 1,1,1,1 }, EAR_ALIGN_TOP_LEFT);

        ear_text(font, "space to start/stop\nr to reset\nleft/right to decrease/increase speed by 1", 0,64, 4,4, (float[4]){ 1,1,1,1 }, EAR_ALIGN_TOP_LEFT);

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

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    eau_delete_clock(clock);

    ear_delete_texture(font);

    eat_stop();

    return 0;
}

