#include "../../eat.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    eat_init("vertex buffer", 1600,900, (eat_init_opts){});

    float data[(2+3)*4] = {
       -.5f, -.5f, 1,0,0,
        .5f, -.5f, 0,1,0,
        .5f, .5f,  0,0,1,
       -.5f, .5f,  1,1,1,
        };

    uint32_t inds[6] = {
        0,1,2,
        2,3,0,
        };

    ear_buffer* vbuf = ear_create_buffer((ear_buffer_desc){
        .type   = EAR_BUF_VERTEX,
        .stride = sizeof(float) * 5,
        }, data, sizeof(data), NULL);

    ear_buffer* ibuf = ear_create_buffer((ear_buffer_desc){
        .type   = EAR_BUF_INDEX,
        .stride = sizeof(uint32_t),
        }, inds, sizeof(inds), NULL);

    // shaders compiled from source to spirv via glslc
    // see command in CMakeLists.txt on ex_vbuffer

    static const char vert[] = {
        #embed "shad_v.spv"
        };
    static const char frag[] = {
        #embed "shad_f.spv"
        };

    ear_pipeline* pln = ear_create_pipeline((ear_pipeline_desc){
        .vertex   = (ear_shader_desc){ .source = vert, .source_size = sizeof(vert) },
        .fragment = (ear_shader_desc){ .source = frag, .source_size = sizeof(frag) },

        .vertex_buffer_amt = 1,
        .vertex_buffers = &vbuf,

        .vertex_attrib_amt = 2,
        .vertex_attribs = (ear_vertex_attrib_desc[]){
            (ear_vertex_attrib_desc){
                .slot       = 0,
                .location   = 0,
                .type       = EAR_PRIM_FLOAT,
                .components = 2,
                .norm       = false,
                .offset     = sizeof(float) * 0,
                },
            (ear_vertex_attrib_desc){
                .slot       = 0,
                .location   = 1,
                .type       = EAR_PRIM_FLOAT,
                .components = 3,
                .norm       = false,
                .offset     = sizeof(float) * 2,
                },
            },
        }, NULL);

    ear_clear_color(NULL, .2f, .4f, .3f, 1);
    while (eat_frame()) {
        ear_bind_pipeline(pln);
        ear_bind_buffer(vbuf, 0, 0);
        ear_bind_buffer(ibuf, 0, 0);
        ear_draw_idx(6, 1);

        printf("%.3f FPS\n", 1./eat_delta64);
    }

    ear_delete_pipeline(pln);
    ear_delete_buffer(ibuf);
    ear_delete_buffer(vbuf);

    eat_exit();

    return 0;
}
