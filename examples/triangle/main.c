#include "../../eat.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    eat_init("triangle", 1600,900, (eat_init_opts){});

    // shaders compiled from source to spirv via glslc
    // see command in CMakeLists.txt on ex_triangle

    static const char vert[] = {
        #embed "shad_v.spv"
        };
    static const char frag[] = {
        #embed "shad_f.spv"
        };

    ear_pipeline* pln = ear_create_pipeline((ear_pipeline_desc){
        .vertex   = (ear_shader_desc){ .source = vert, .source_size = sizeof(vert) },
        .fragment = (ear_shader_desc){ .source = frag, .source_size = sizeof(frag) },
        }, NULL);

    while (eat_frame()) {
        ear_clear_color(.2f, .4f, .3f);

        ear_bind_pipeline(pln);
        ear_draw(3, 1);

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    ear_delete_pipeline(pln);

    eat_exit();

    return 0;
}
