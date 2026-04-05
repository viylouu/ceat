#include "../../eat.h"

#include <stdio.h>

int main(void) {
    eat_init("window", 1600,900, (eat_init_opts){});

    while (eat_frame()) {
        ear_clear(.2f, .4f, .3f);

        ear_rect(0,0, 64,64, (float[4]){ 1,0,0,1 }, EAU_ALIGN_TOP_LEFT);

        //printf("%.3f FPS\n", 1./eat_delta64);
    }

    eat_exit();

    return 0;
}
