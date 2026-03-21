#include "misc.h"
#include "../cutil.h"

#include "../eaw/window.h"
#include "user.h"
#include "gl.h"
#include "framebuffer.h"

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
ear_mask(
    float x, float y, float w, float h
    ) {
    ear_flush();

    if (_ear_cur_framebuffer == NULL) {
        gl.scissor(x,eaw_window_height - (y+h), w, h);
    } else {
        gl.scissor(x,_ear_cur_framebuffer->desc.height - (y+h), w, h);
    }
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
