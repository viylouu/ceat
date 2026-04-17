#include "screen.h"

#include "../init/funcs.h"
#include "buffer.h"
#include "framebuffer.h"
#include "../util/screen.h"
#include "pipeline.h"

#include "ear/ear.h"

void
_ear_gl_clear(
    void
    ) {
    if (_ear_gl_cur_fb != NULL) gl.clearColor(
        _ear_gl_cur_fb->r, 
        _ear_gl_cur_fb->g, 
        _ear_gl_cur_fb->b, 
        _ear_gl_cur_fb->a
        );

    if (_ear_gl_cur_fb == NULL && !_ear_clear_this_frame) return;

    gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
ear_gl_clear_color(
    void* _fb,
    float r, float g, float b, float a
    ) {
    if (_fb == NULL) gl.clearColor(r,g,b,a);
    else {
        ear_gl_framebuffer* fb = _fb;
        fb->r = r;
        fb->g = g;
        fb->b = b;
        fb->a = a;
    }
}

void
ear_gl_set_viewport(
    float x, float y,
    float w, float h
    ) {
    gl.viewport(x,y,w,h);
}
void
ear_gl_set_scissor(
    float x, float y,
    float w, float h
    ) {
    gl.scissor(x,y,w,h);
}

void
ear_gl_draw(
    uint32_t vertices, uint32_t instances
    ) {
    gl.drawArraysInstanced(_ear_gl_convert_draw_mode(_ear_gl_cur_pln->desc.topology), 0, vertices, instances);
}
void
ear_gl_draw_idx(
    uint32_t indices, uint32_t instances
    ) {
    gl.drawElementsInstanced(_ear_gl_convert_draw_mode(_ear_gl_cur_pln->desc.topology), indices, GL_UNSIGNED_INT, _ear_gl_index_offset, instances);
}
