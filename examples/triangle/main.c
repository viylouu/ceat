#include "../../eat.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    eat_init("triangle", 1600,900, (eat_init_opts){});

    char* vertex = eau_load_file("examples/triangle/shad.vert", NULL);
    char* fragment = eau_load_file("examples/triangle/shad.frag", NULL);

    ear_pipeline* pln = ear_create_pipeline((ear_pipeline_desc){
            .vertex = (ear_shader_desc){ .source = vertex },
            .fragment = (ear_shader_desc){ .source = fragment },
        }, NULL);

    free(vertex);
    free(fragment);

    while (eat_frame()) {
        ear_clear((float[3]){ .2f, .4f, .3f });

        ear_bind_pipeline(pln);
        ear_draw(3, 1, EAR_MODE_TRIANGLES);

        printf("%.3f FPS\n", 1.f/eat_delta);
    }

    ear_delete_pipeline(pln);

    eat_stop();

    return 0;
}
