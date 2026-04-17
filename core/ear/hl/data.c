#include "data.h"
#include "../../cutil.h"

#include "../../eau/arena.h"

#include "../ll/misc.h"

#include "../backends/rendering/impl.h"

mat4 proj;
mat4 transf;

eau_arena* arena;
eau_arena** _ear_data_arena = &arena;

ear_rect_rend ear_rr;

void
ear_rect_rend_create(
    void
    ) {
    static const char vert_g[] = {
        #embed "shaders/rect.vert"
        ,0
        };
    static const char frag_g[] = {
        #embed "shaders/rect.frag"
        ,0
        };
    static const char vert_spv[] = {
        #embed "shaders/rect_v.spv"
        };
    static const char frag_spv[] = {
        #embed "shaders/rect_f.spv"
        };

    const char* vert = ear_backend->deps.use_spirv? vert_spv : vert_g;
        uint32_t vert_size = ear_backend->deps.use_spirv? sizeof(vert_spv) : sizeof(vert_g);
    const char* frag = ear_backend->deps.use_spirv? frag_spv : frag_g;
        uint32_t frag_size = ear_backend->deps.use_spirv? sizeof(frag_spv) : sizeof(frag_g);

    ear_rr.off = 0;

    ear_rr.ubo = ear_create_buffer((ear_buffer_desc){
        .type   = EAR_BUF_UNIFORM,
        .stride = sizeof(ear_rr.ubo_d),
        .chunk_size = 1,
        }, &ear_rr.ubo_d, sizeof(ear_rr.ubo_d) * EAR_HL_RINGBUF_MULT, arena);

    ear_rr.ssbo = ear_create_buffer((ear_buffer_desc){
        .type   = EAR_BUF_STORAGE,
        .stride = sizeof(ear_rr.ssbo_d[0]),
        .chunk_size = EAR_HL_BATCH_OBJS,
        }, &ear_rr.ssbo_d, sizeof(ear_rr.ssbo_d) * EAR_HL_RINGBUF_MULT, arena);

    ear_rr.set = ear_create_bindset((ear_bindset_desc){
        .binding_amt = 2,
        .bindings    = (ear_bind_desc[]){
            (ear_bind_desc){
                .object  = ear_rr.ssbo,
                .type    = EAR_BIND_STORAGE,
                .binding = 0,
                .stage   = EAR_STAGE_VERTEX,
                },
            (ear_bind_desc){
                .object  = ear_rr.ubo,
                .type    = EAR_BIND_UNIFORM,
                .binding = 1,
                .stage   = EAR_STAGE_VERTEX,
                },
            },
        }, arena);

    ear_rr.pln = ear_create_pipeline((ear_pipeline_desc){
        .vertex   = (ear_shader_desc){ .source = vert, .source_size = vert_size },
        .fragment = (ear_shader_desc){ .source = frag, .source_size = frag_size },

        .bindset_amt = 1,
        .bindsets    = &ear_rr.set,

        .has_blend_state = true,
        .blend_state     = (ear_blend_state){ 
            .src_color = EAR_FAC_SRC_ALPHA, .dst_color = EAR_FAC_INV_SRC_ALPHA,
            .src_alpha = EAR_FAC_ONE,       .dst_alpha = EAR_FAC_INV_SRC_ALPHA,
            .color_op  = EAR_OP_ADD, .alpha_op = EAR_OP_ADD,
            },

        .color_fmt_amt = 1,
        .color_fmts    = (ear_texture_type[]){ EAR_TEX_COLOR },
        }, arena);
}

void
ear_rect_rend_flush(
    void
    ) {
    if (ear_rr.ssbo_i == 0) return;

    eau_mat4_copy(proj, &ear_rr.ubo_d.proj);

    ear_update_buffer(ear_rr.ubo, ear_rr.off);
    ear_update_buffer(ear_rr.ssbo, ear_rr.off);

    ear_bind_pipeline(ear_rr.pln);
    ear_bind_bindset(ear_rr.set, 0, (uint32_t[2]){ear_rr.off, ear_rr.off});

    ear_draw(6, ear_rr.ssbo_i);

    ear_rr.ssbo_i = 0;

    ++ear_rr.off;
    if (ear_rr.off >= EAR_HL_RINGBUF_MULT) ear_rr.off = 0;
}


ear_tex_rend ear_tr;

void
ear_tex_rend_create(
    void
    ) {
    static const char vert_g[] = {
        #embed "shaders/tex.vert"
        ,0
        };
    static const char frag_g[] = {
        #embed "shaders/tex.frag"
        ,0
        };
    static const char vert_spv[] = {
        #embed "shaders/tex_v.spv"
        };
    static const char frag_spv[] = {
        #embed "shaders/tex_f.spv"
        };

    const char* vert = ear_backend->deps.use_spirv? vert_spv : vert_g;
        uint32_t vert_size = ear_backend->deps.use_spirv? sizeof(vert_spv) : sizeof(vert_g);
    const char* frag = ear_backend->deps.use_spirv? frag_spv : frag_g;
        uint32_t frag_size = ear_backend->deps.use_spirv? sizeof(frag_spv) : sizeof(frag_g);

    ear_tr.off = 0;

    ear_tr.ubo = ear_create_buffer((ear_buffer_desc){
        .type   = EAR_BUF_UNIFORM,
        .stride = sizeof(ear_tr.ubo_d),
        .chunk_size = 1,
        }, &ear_tr.ubo_d, sizeof(ear_tr.ubo_d) * EAR_HL_RINGBUF_MULT, arena);

    ear_tr.ssbo = ear_create_buffer((ear_buffer_desc){
        .type   = EAR_BUF_STORAGE,
        .stride = sizeof(ear_tr.ssbo_d[0]),
        .chunk_size = EAR_HL_BATCH_OBJS,
        }, &ear_tr.ssbo_d, sizeof(ear_tr.ssbo_d) * EAR_HL_RINGBUF_MULT, arena);

    ear_tr.set = ear_create_bindset((ear_bindset_desc){
        .binding_amt = 2,
        .bindings    = (ear_bind_desc[]){
            (ear_bind_desc){
                .object  = ear_tr.ssbo,
                .type    = EAR_BIND_STORAGE,
                .binding = 0,
                .stage   = EAR_STAGE_VERTEX,
                },
            (ear_bind_desc){
                .object  = ear_tr.ubo,
                .type    = EAR_BIND_UNIFORM,
                .binding = 1,
                .stage   = EAR_STAGE_VERTEX,
                },
            },
        }, arena);

    ear_texture* temp = ear_create_texture((ear_texture_desc){}, NULL,16,16, NULL);
    ear_bindset* tbind = ear_create_bindset((ear_bindset_desc){
        .binding_amt = 1,
        .bindings    = &(ear_bind_desc){
            .object  = temp,
            .type    = EAR_BIND_TEXTURE2D,
            .binding = 0,
            .stage   = EAR_STAGE_FRAGMENT,
            },
        }, arena);

    ear_tr.pln = ear_create_pipeline((ear_pipeline_desc){
        .vertex   = (ear_shader_desc){ .source = vert, .source_size = vert_size },
        .fragment = (ear_shader_desc){ .source = frag, .source_size = frag_size },

        .bindset_amt = 2,
        .bindsets    = (ear_bindset*[]){ 
            ear_tr.set,
            tbind,
            },

        .has_blend_state = true,
        .blend_state     = (ear_blend_state){ 
            .src_color = EAR_FAC_SRC_ALPHA, .dst_color = EAR_FAC_INV_SRC_ALPHA,
            .src_alpha = EAR_FAC_ONE,       .dst_alpha = EAR_FAC_INV_SRC_ALPHA,
            .color_op  = EAR_OP_ADD, .alpha_op = EAR_OP_ADD,
            },

        .color_fmt_amt = 1,
        .color_fmts    = (ear_texture_type[]){ EAR_TEX_COLOR },
        }, arena);

    ear_delete_bindset(tbind);
    ear_delete_texture(temp);
}

void
ear_tex_rend_flush(
    void
    ) {
    if (ear_tr.ssbo_i == 0) return;

    eau_mat4_copy(proj, &ear_tr.ubo_d.proj);

    ear_update_buffer(ear_tr.ubo, ear_tr.off);
    ear_update_buffer(ear_tr.ssbo, ear_tr.off);

    eat_assert(ear_tr.cur_tex->hl_bindset != NULL, "texture has no hl bindset!");

    ear_bind_pipeline(ear_tr.pln);
    ear_bind_bindset(ear_tr.set, 0, (uint32_t[2]){ear_tr.off, ear_tr.off});
    ear_bind_bindset(ear_tr.cur_tex->hl_bindset, 1, NULL);

    ear_draw(6, ear_tr.ssbo_i);

    ear_tr.ssbo_i = 0;

    ++ear_tr.off;
    if (ear_tr.off >= EAR_HL_RINGBUF_MULT) ear_tr.off = 0;
}


void
ear_user_init(
    void
    ) {
    arena = eau_create_arena();

    ear_rect_rend_create();
    ear_tex_rend_create();
}

void 
ear_user_exit(
    void
    ) {
    eau_delete_arena(arena);
    //ear_rect_rend_delete();
    //ear_tex_rend_delete();
}
