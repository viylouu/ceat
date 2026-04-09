#include "../../eat.h"

#include <stdio.h>

int main(void) {
    eat_init("camera", 1600,900, (eat_init_opts){});

    float x = 0; float y = 0;
    float rot = 0;

    ear_camera* cam = ear_create_camera((ear_camera_desc){
        .fb = NULL,
        .type = EAR_CAMERA_2D,
        .desc_2d = (ear_camera_2d_desc){
            .x = 0, .y = 0,
            .scalex = 1, .scaley = 1,
            .rotation = 0,
            },
        }, NULL);

    while (eat_frame()) {
        if (eaw_is_key(EAW_KEY_LEFT)) rot += eat_delta * 2;
        if (eaw_is_key(EAW_KEY_RIGHT)) rot -= eat_delta * 2;

        if (eaw_is_key(EAW_KEY_W)) y -= eat_delta * 128;
        if (eaw_is_key(EAW_KEY_S)) y += eat_delta * 128;
        if (eaw_is_key(EAW_KEY_A)) x -= eat_delta * 128;
        if (eaw_is_key(EAW_KEY_D)) x += eat_delta * 128;

        ear_set_camera_rotation_2d(cam, rot);
        ear_set_camera_position_2d(cam, x,y);

        ear_bind_camera(cam, false);
        ear_clear_color(NULL, .2f, .4f, .3f, 1);
        ear_rect(0,0, 64,64, (float[4]){ 1,0,0,1 }, EAU_ALIGN_MID);
        ear_rect(256,256, 128,128, (float[4]){ 1,0,0,1 }, EAU_ALIGN_MID);

        ear_bind_camera(cam, true);
        ear_rect(0,0, 64,64, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    ear_delete_camera(cam);

    eat_exit();

    return 0;
}
