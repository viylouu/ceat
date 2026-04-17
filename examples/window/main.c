#include "../../eat.h"

#include "../../backends/rendering/opengl/gl.h"
#include "../../backends/windowing/glfw/glfw.h"

#include <stdio.h>

void game_init(void) {

}
void game_exit(void) {

}

int main(void) {
    // plugins can be specified in whatever order for the most part
    // game plugins always init last
    // backend plugins always init first and update first
    // plugins depending on other plugins init after the deps
    // update order is decided by plugin order however
    eat_run(
        "window",
        1600, 900,
        false,

        (const eat_plugin*[]){
            &(eat_plugin){ 
                .name = "goon",
                .type = EAT_PLUG_GENERIC,

                .init = game_init,
                .exit = game_exit,
                },
            &ear_gl_plugin,
            &eaw_glfw_plugin,
            }, 3
        );

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
