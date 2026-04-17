#include "../../eat.h"

#include "../../backends/rendering/opengl/gl.h"

#include <stdio.h>

void game_init(void) {

}
void game_exit(void) {

}

int main(void) {
    eat_run((const eat_plugin*[]){
        &(eat_plugin){ 
            .name = "goon",
            .init = game_init,
            .exit = game_exit,
            },
        }, 1);

    /*
    eat_init("window", 1600,900, (eat_init_opts){ .rendering_impl = &ear_gl_impl });

    ear_clear_color(NULL, .2f, .4f, .3f, 1);
    while (eat_frame()) {
        ear_rect(0,0, 64,64, (float[4]){ 1,0,0,1 }, EAU_ALIGN_TOP_LEFT);

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    eat_exit();
    */

    return 0;
}
