#include "data.h"
//#include "../cutil.h"

#include "../../eau/arena.h"

#include "../ll/misc.h"

mat4 proj;
mat4 transf;

eau_arena* arena;

ear_rect_rend ear_rr;

void
ear_rect_rend_create(
    void
    ) {
    static const char vert[] = {
        #embed "shaders/rect_v.spv"
        };
    static const char frag[] = {
        #embed "shaders/rect_f.spv"
        };

    ear_rr.ubo = ear_create_buffer((ear_buffer_desc){
        .type   = EAR_BUF_UNIFORM,
        .stride = sizeof(ear_rr.ubo_d),
        }, &ear_rr.ubo_d, sizeof(ear_rr.ubo_d), arena);

    ear_rr.ssbo = ear_create_buffer((ear_buffer_desc){
        .type   = EAR_BUF_STORAGE_PERSISTENT,
        .stride = sizeof(ear_rr.ssbo_d[0]),
        }, &ear_rr.ssbo_d, sizeof(ear_rr.ssbo_d), arena);

    ear_rr.set = ear_create_bindset((ear_bindset_desc){
        .binding_amt = 2,
        .bindings    = (ear_bind_desc[]){
            (ear_bind_desc){
                .buffer  = ear_rr.ssbo,
                .binding = 0,
                .stage   = EAR_STAGE_VERTEX,
                },
            (ear_bind_desc){
                .buffer  = ear_rr.ubo,
                .binding = 1,
                .stage   = EAR_STAGE_VERTEX,
                },
            },
        }, arena);

    ear_attach_buffer_bindset(ear_rr.ubo,  ear_rr.set);
    ear_attach_buffer_bindset(ear_rr.ssbo, ear_rr.set);

    ear_rr.pln = ear_create_pipeline((ear_pipeline_desc){
        .vertex   = (ear_shader_desc){ .source = vert, .source_size = sizeof(vert) },
        .fragment = (ear_shader_desc){ .source = frag, .source_size = sizeof(frag) },

        .bindset_amt = 1,
        .bindsets    = &ear_rr.set,

        .has_blend_state = true,
        .blend_state     = (ear_blend_state){ 
            .src_color = EAR_FAC_SRC_ALPHA, .dst_color = EAR_FAC_INV_SRC_ALPHA,
            .src_alpha = EAR_FAC_ONE,       .dst_alpha = EAR_FAC_INV_SRC_ALPHA,
            .color_op  = EAR_OP_ADD, .alpha_op = EAR_OP_ADD,
            },
        }, arena);
}

void
ear_rect_rend_flush(
    void
    ) {
    if (ear_rr.ssbo_i == 0) return;

    eau_mat4_copy(proj, &ear_rr.ubo_d.proj);

    ear_update_buffer(ear_rr.ubo);
    ear_update_buffer(ear_rr.ssbo);

    ear_bind_pipeline(ear_rr.pln);
    //ear_bind_buffer(ear_rr.ssbo, 0);
    //ear_bind_buffer(ear_rr.ubo, 1);
    ear_bind_bindset(ear_rr.set);

    ear_draw(6, ear_rr.ssbo_i);

    ear_rr.ssbo_i = 0;
}


/*
ear_tex_rend ear_tr;

void
ear_tex_rend_create(
    void
    ) {
    static const char vert[] = {
    #embed "shaders/tex.vert"
    , 0
    };
    static const char frag[] = {
    #embed "shaders/tex.frag"
    , 0
    };

    ear_tr.pln = ear_create_pipeline((ear_pipeline_desc){
            .vertex = (ear_shader_desc){ .source = (char*)vert },
            .fragment = (ear_shader_desc){ .source = (char*)frag },
            .blend_state = { true, (ear_blend_state){ 
                .src_color = EAR_FAC_SRC_ALPHA, .dst_color = EAR_FAC_INV_SRC_ALPHA,
                .src_alpha = EAR_FAC_ONE,       .dst_alpha = EAR_FAC_INV_SRC_ALPHA,
                .color_op = EAR_OP_ADD, .alpha_op = EAR_OP_ADD,
            }, },
        }, arena);

    ear_tr.ubo = ear_create_buffer((ear_buffer_desc){
            .type = EAR_BUF_UNIFORM,
            .usage = EAR_USAGE_DYNAMIC,
            .stride = sizeof(ear_tr.ubo_d),
        }, &ear_tr.ubo_d, sizeof(ear_tr.ubo_d), arena);

    ear_tr.ssbo = ear_create_buffer((ear_buffer_desc){
            .type = EAR_BUF_STORAGE,
            .usage = EAR_USAGE_DYNAMIC,
            .stride = sizeof(ear_tr.ssbo_d[0]),
        }, &ear_tr.ssbo_d, sizeof(ear_tr.ssbo_d), arena);
}

void
ear_tex_rend_flush(
    void
    ) {
    if (ear_tr.ssbo_i == 0) return;

    eau_mat4_copy(proj, &ear_tr.ubo_d.proj);

    ear_update_buffer(ear_tr.ubo);
    ear_update_buffer(ear_tr.ssbo);

    ear_bind_pipeline(ear_tr.pln);
    ear_bind_buffer(ear_tr.ssbo, 0);
    ear_bind_buffer(ear_tr.ubo, 1);
    ear_bind_texture(ear_tr.cur_tex, 2);

    ear_draw(6, ear_tr.ssbo_i, EAR_MODE_TRIANGLES);

    ear_tr.ssbo_i = 0;
}
*/


void
ear_user_init(
    void
    ) {
    arena = eau_create_arena();

    ear_rect_rend_create();
    //ear_tex_rend_create();
}

void 
ear_user_exit(
    void
    ) {
    eau_delete_arena(arena);
    //ear_rect_rend_delete();
    //ear_tex_rend_delete();
}
