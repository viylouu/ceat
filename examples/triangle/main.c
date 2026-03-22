#include "../../eat.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    eat_init("triangle", 1600,900, (eat_init_opts){});

    // shaders compiled from source to spirv via glslc

    uint32_t vertex_len; uint32_t fragment_len;
    char* vertex   = eau_load_file("examples/triangle/shad_v.spv", &vertex_len);
    char* fragment = eau_load_file("examples/triangle/shad_f.spv", &fragment_len);

    ear_pipeline* pln = ear_create_pipeline((ear_pipeline_desc){
            .vertex   = (ear_shader_desc){ .source = vertex,   .source_size = vertex_len },
            .fragment = (ear_shader_desc){ .source = fragment, .source_size = fragment_len },
        }, NULL);

    free(vertex);
    free(fragment);

    while (eat_frame()) {
        //ear_clear((float[3]){ .2f, .4f, .3f });

        ear_bind_pipeline(pln);
        ear_draw(3, 1);

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    ear_delete_pipeline(pln);

    eat_exit();

    return 0;
}
