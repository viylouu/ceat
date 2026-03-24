#include "../../eat.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    eat_init("triangle", 1600,900, (eat_init_opts){});

    struct{
        float time;
    } ubo_data = {};

    ear_buffer* ubo = ear_create_buffer((ear_buffer_desc){
        .type = EAR_BUF_UNIFORM,
        .stride = sizeof(ubo_data),
        }, &ubo_data, sizeof(ubo_data), NULL);

    // shaders compiled from source to spirv via glslc
    // see command in CMakeLists.txt on ex_triangle

    uint32_t vertex_len; uint32_t fragment_len;
    char* vertex   = eau_load_file("examples/triangle/shad_v.spv", &vertex_len);
    char* fragment = eau_load_file("examples/triangle/shad_f.spv", &fragment_len);

    ear_pipeline* pln = ear_create_pipeline((ear_pipeline_desc){
        .vertex   = (ear_shader_desc){ .source = vertex,   .source_size = vertex_len },
        .fragment = (ear_shader_desc){ .source = fragment, .source_size = fragment_len },

        // sets are used to group similar data in the pipeline for update speed
        .buffer_attrib_set_amt = 1,
        .buffer_attrib_sets = &(ear_buffer_attrib_desc_set){
            .buffer_attrib_amt = 1,
            .buffer_attribs = &(ear_buffer_attrib_desc){
                .type = EAR_BUF_UNIFORM,
                .binding = 0,
                .stage = EAR_STAGE_VERTEX,
                },
            },
        }, NULL);

    free(vertex);
    free(fragment);

    while (eat_frame()) {
        //ear_clear((float[3]){ .2f, .4f, .3f });

        ubo_data.time = eat_time;
        ear_update_buffer(ubo);

        ear_bind_pipeline(pln);
        ear_bind_buffer(ubo, 0);
        ear_draw(3, 1);

        //printf("%.3f FPS\n", 1./eat_delta64);
    }

    ear_delete_pipeline(pln);
    ear_delete_buffer(ubo);

    eat_exit();

    return 0;
}
