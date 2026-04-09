#include "../../eat.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    eat_init("triangle", 1600,900, (eat_init_opts){});

    struct{
        float time;
    } ubo_data = {};
    
    ear_buffer* ubo = ear_create_buffer((ear_buffer_desc){
        .type   = EAR_BUF_UNIFORM_PERSISTENT,
        .stride = sizeof(ubo_data),
        }, &ubo_data, sizeof(ubo_data), NULL);

    ear_bindset* set = ear_create_bindset((ear_bindset_desc){
        .binding_amt = 1,
        .bindings    = &(ear_bind_desc){
            .object  = ubo,
            .type    = EAR_BIND_UNIFORM,
            .binding = 0,
            .stage   = EAR_STAGE_VERTEX,
            },
        }, NULL);

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

        .bindset_amt = 1,
        .bindsets    = &set,
        }, NULL);

    while (eat_frame()) {
        ear_clear_color(NULL, .2f, .4f, .3f, 1);

        ubo_data.time = eat_time;
        ear_update_buffer(ubo, 0);

        ear_bind_pipeline(pln);
        ear_bind_bindset(set, 0, NULL,0);
        ear_draw(3, 1);

        //printf("%.3f FPS\n", 1./eat_delta64);
    }

    ear_delete_pipeline(pln);
    ear_delete_bindset(set);
    ear_delete_buffer(ubo);

    eat_exit();

    return 0;
}
