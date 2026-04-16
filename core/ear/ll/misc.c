#include "misc.h"
//#include "../../cutil.h"

//#include <math.h>

#include "../../eaw/window.h"
#include "../hl/user.h"
#include "framebuffer.h"

#include "rendering/impl.h"

extern ear_framebuffer* _eat_screen_framebuffer;

void
ear_clear_color(
    ear_framebuffer* fb,
    float r, float g, float b, float a
    ) {
    if (fb == NULL) fb = _ear_default_fb;
    if (fb == NULL) fb = _ear_master_fb;
    if (fb == NULL) fb = _eat_screen_framebuffer; // assume user-end call
    ear_backend->misc.clear(fb == NULL? NULL : fb->vk, r,g,b,a);
}

void
ear_draw(
    uint32_t vertices, uint32_t instances
    ) {
    ear_backend->misc.draw(vertices, instances);
}
void
ear_draw_idx(
    uint32_t indices, uint32_t instances
    ) {
    ear_backend->misc.draw_idx(indices, instances);
}

void
ear_mask(
    float x, float y, float w, float h
    ) {
    ear_flush();

    if (_ear_cur_framebuffer == NULL) ear_backend->misc.scissor(x,_eaw_window_height - (y+h), w, h);
    else ear_backend->misc.scissor(x,_ear_cur_framebuffer->desc.height - (y+h), w, h);
}
