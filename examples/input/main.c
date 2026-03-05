#include "../../eat.h"

#include <stdio.h>

int main(void) {
    eat_init("input", 1600,900, (eat_init_opts){});

    float x = 0; float y = 0;

    while (eat_frame()) {
        ear_clear((float[3]){ .2f, .4f, .3f });

        const float speed = 256;

        if (eaw_is_key(EAW_KEY_LEFT)) x -= eat_delta * speed;
        if (eaw_is_key(EAW_KEY_RIGHT)) x += eat_delta * speed;
        if (eaw_is_key(EAW_KEY_UP)) y -= eat_delta * speed;
        if (eaw_is_key(EAW_KEY_DOWN)) y += eat_delta * speed;

        ear_rect(x,y, 64,64, eaw_is_mouse(EAW_MOUSE_LEFT)? (float[4]){ 0,1,0,1 } : (float[4]){ 1,0,0,1 });

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    eat_stop();

    return 0;
}
