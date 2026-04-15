#include "user.h"
//#include "../../cutil.h"

#include "data.h"
#include "camera.h"

#include <string.h>

void
ear_flush(
    void
    ) {
    ear_rect_rend_flush();
    ear_tex_rend_flush();
}


enum _ear_int__last_used{
    EAR_INT_LU_RECT,
    EAR_INT_LU_TEX,
};
enum _ear_int__last_used last_used;

void
ear_rect(
    float x, float y,
    float w, float h,
    float col[4],
    eau_align align
    ) {
    if (ear_rr.ssbo_i == sizeof(ear_rr.ssbo_d)/sizeof(ear_rr.ssbo_d[0])) ear_rect_rend_flush();
    if (last_used != EAR_INT_LU_RECT) ear_flush();
    last_used = EAR_INT_LU_RECT;

    eau_rect rect = (eau_rect){ x,y,w,h, align };
    rect = eau_rect_topleftify(rect);

    mat4 mat;
    if (!ear_bound_camera_ui_mode && ear_bound_camera)
        eau_mat4_mult(&mat, transf, ear_bound_camera->matrix);
    else
        eau_mat4_copy(transf, &mat);

    ear_rr.ssbo_d[ear_rr.ssbo_i] = (typeof(ear_rr.ssbo_d[0])){ 
        rect.x,rect.y,
        rect.w,rect.h, 
        col[0],col[1],col[2],col[3],
        {0},
        };
    memcpy((uint8_t*)(&ear_rr.ssbo_d[ear_rr.ssbo_i]) + sizeof(ear_rr.ssbo_d[0]) - sizeof(float)*16, mat, sizeof(float)*16);
    ++ear_rr.ssbo_i;
}

void
ear_tex(
    ear_texture* tex,
    float x, float y,
    float w, float h,
    float sx, float sy, float sw, float sh,
    float col[4],
    eau_align align
    ) {
    if (ear_tr.ssbo_i == sizeof(ear_tr.ssbo_d)/sizeof(ear_tr.ssbo_d[0])) ear_tex_rend_flush();
    if (last_used != EAR_INT_LU_TEX) ear_flush();
    last_used = EAR_INT_LU_TEX;
    if (ear_tr.cur_tex != tex) {
        ear_tex_rend_flush();

        ear_tr.cur_tex = tex;
        if (tex->hl_bindset == NULL)
            tex->hl_bindset = ear_create_bindset((ear_bindset_desc){
                .binding_amt = 1,
                .bindings    = &(ear_bind_desc){
                    .object  = tex,
                    .type    = EAR_BIND_TEXTURE2D,
                    .binding = 0,
                    .stage   = EAR_STAGE_FRAGMENT,
                    },
                }, *_ear_data_arena);
    }

    eau_rect rect = (eau_rect){ x,y,w,h, align };
    rect = eau_rect_topleftify(rect);

    mat4 mat;
    if (!ear_bound_camera_ui_mode && ear_bound_camera)
        eau_mat4_mult(&mat, transf, ear_bound_camera->matrix);
    else
        eau_mat4_copy(transf, &mat);

    ear_tr.ssbo_d[ear_tr.ssbo_i] = (typeof(ear_tr.ssbo_d[0])){ 
        rect.x,rect.y,
        rect.w,rect.h,
        col[0],col[1],col[2],col[3],
        (float)sx/tex->width,1-(float)(sh+sy)/tex->height,(float)sw/tex->width,(float)sh/tex->height,
        {0},
        };
    memcpy((uint8_t*)(&ear_tr.ssbo_d[ear_tr.ssbo_i]) + sizeof(ear_tr.ssbo_d[0]) - sizeof(float)*16, mat, sizeof(float)*16);
    ++ear_tr.ssbo_i;
}


void
ear_translate(
    float x, float y
    ) {
    mat4 trans;
    eau_mat4_translate(&trans, x,y,0);
    eau_mat4_mult(&transf, trans, transf);
}

void
ear_scale(
    float x, float y
    ) {
    mat4 scale;
    eau_mat4_scale(&scale, x,y,1);
    eau_mat4_mult(&transf, scale, transf);
}

void
ear_rotate(
    float ang
    ) {
    mat4 rot;
    eau_mat4_rotate_z(&rot, ang);
    eau_mat4_mult(&transf, rot, transf);
}

void
ear_rev_translate(
    float x, float y
    ) {
    mat4 trans;
    eau_mat4_translate(&trans, x,y,0);
    eau_mat4_mult(&transf, transf, trans);
}

void
ear_rev_scale(
    float x, float y
    ) {
    mat4 scale;
    eau_mat4_scale(&scale, x,y,1);
    eau_mat4_mult(&transf, transf, scale);
}

void
ear_rev_rotate(
    float ang
    ) {
    mat4 rot;
    eau_mat4_rotate_z(&rot, ang);
    eau_mat4_mult(&transf, transf, rot);
}

void
ear_reset_transform(
    void
    ) {
    eau_mat4_identity(&transf);
}

void
ear_save_transform(
    mat4* out
    ) {
    eau_mat4_copy(transf, out);
}

void
ear_load_transform(
    mat4 mat
    ) {
    eau_mat4_copy(mat, &transf);
}
