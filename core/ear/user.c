#include "user.h"
#include "../cutil.h"

#include "gl.h"
#include "data.h"

#include <GL/gl.h>
#include <GL/glext.h>

GLenum
_TYPECONV_draw_mode(
    ear_draw_mode mode
    );


void 
ear_clear(
    float col[3]
    ) {
    gl.clearColor(col[0], col[1], col[2], 1);
    gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
ear_draw(
    int32_t vertices, int32_t instances, ear_draw_mode draw_mode
    ) {
    gl.drawArraysInstanced(_TYPECONV_draw_mode(draw_mode), 0, vertices, instances);
}

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
    float col[4]
    ) {
    if (ear_rr.ssbo_i == sizeof(ear_rr.ssbo_d)/sizeof(ear_rr.ssbo_d[0])) ear_rect_rend_flush();
    if (last_used != EAR_INT_LU_RECT) ear_flush();
    last_used = EAR_INT_LU_RECT;

    ear_rr.ssbo_d[ear_rr.ssbo_i] = (typeof(ear_rr.ssbo_d[0])){ x,y, w,h, col[0],col[1],col[2],col[3] };
    ++ear_rr.ssbo_i;
}

void
ear_tex(
    ear_texture* tex,
    float x, float y,
    float w, float h,
    float sx, float sy, float sw, float sh,
    float col[4]
    ) {
    if (ear_tr.ssbo_i == sizeof(ear_tr.ssbo_d)/sizeof(ear_tr.ssbo_d[0])) ear_tex_rend_flush();
    if (last_used != EAR_INT_LU_TEX) ear_flush();
    last_used = EAR_INT_LU_TEX;
    if (ear_tr.cur_tex != tex) ear_tex_rend_flush();
    ear_tr.cur_tex = tex;

    ear_tr.ssbo_d[ear_tr.ssbo_i] = (typeof(ear_tr.ssbo_d[0])){ x,y, w,h, col[0],col[1],col[2],col[3], (float)sx/tex->width,1-(float)(sh+sy)/tex->height,(float)sw/tex->width,(float)sh/tex->height };
    ++ear_tr.ssbo_i;
}




GLenum
_TYPECONV_draw_mode(
    ear_draw_mode mode
    ) {
    switch (mode) {
    case EAR_MODE_TRIANGLES: return GL_TRIANGLES;
    case EAR_MODE_LINES:     return GL_LINES;
    }

    eat_unreachable();
}
