#include "../../eat.h"

#include <stdio.h>

int main(void) {
    eat_init("audio", 1600,900, (eat_init_opts){});

    static const uint8_t audio_data[] = {
    #embed "sound.wav"
    };

    eaa_sound* sound = eaa_load_sound((eaa_sound_desc){
            .volume = 1,
            .pitch = 1,
            .loop = false,
            .mixer = NULL,
        }, audio_data, sizeof(audio_data), NULL);


    while (eat_frame()) {
        ear_clear((float[3]){ .2f, .4f, .3f });

        ear_rect(0,0, 64,64, (float[4]){ 1,0,0,1 }, EAU_ALIGN_TOP_LEFT);

        if (eaw_is_key_pressed(EAW_KEY_SPACE)) eaa_play_sound(sound);

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    eaa_delete_sound(sound);

    eat_stop();

    return 0;
}

