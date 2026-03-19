#include "../../eat.h"

#include <stdio.h>

void onzero(void* data) {
    printf(":3\n");
}

int main(void) {
    eat_init("timer", 1600,900, (eat_init_opts){});

    static const char tex_data[] = {
    #embed "font.png"
    };

    ear_font* font = ear_load_bitmap_mono_font(tex_data, sizeof(tex_data), NULL);

    eau_timer* timer = eau_create_timer(1, false, NULL);

    eau_set_timer_onzero(timer, onzero, NULL);

    bool paused = true;
    float speed = 1;

    while (eat_frame()) {
        ear_clear((float[3]){ .2f, .4f, .3f });

        eau_update_timers();

        char buf[64];
        snprintf(buf, sizeof(buf), "%.3f", timer->time);

        ear_text(font, buf, 0,0, 8 * 4, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        ear_text(font, "space to start/stop\nr to reset\nleft/right to decrease/increase speed by 1", 0,64, 8 * 4, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        if (eaw_is_key_pressed(EAW_KEY_SPACE)) {
            paused = !paused;
            if (!paused) eau_start_timer(timer);
            else eau_stop_timer(timer);
        }

        if (eaw_is_key_pressed(EAW_KEY_R)) {
            eau_reset_timer(timer);
        }

        if (eaw_is_key_pressed(EAW_KEY_LEFT)) {
            speed -= 1;
            eau_set_timer_speed(timer, speed);
        } else if(eaw_is_key_pressed(EAW_KEY_RIGHT)) {
            speed += 1;
            eau_set_timer_speed(timer, speed);
        }

        //printf("%.3f FPS\n", 1./eat_delta64);
    }

    eau_delete_timer(timer);

    ear_delete_font(font);

    eat_stop();

    return 0;
}

