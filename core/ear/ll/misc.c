#include "misc.h"
#include "../../cutil.h"

#include <math.h>

//#include "../../eaw/window.h"
//#include "user.h"
//#include "framebuffer.h"

#include "framebuffer.h"
#include "vk/sc/render_pass.h"
#include "vk/sc/swapchain.h"
#include "vk/init/comm_buffer.h"
#include "vk/eng/framebuffer.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

float _ear_clear_color[3] = {0};

void
ear_clear_color(
    ear_framebuffer* fb,
    float r, float g, float b, float a
    ) {
    if (fb == NULL) fb = _ear_default_fb;
    if (fb == NULL) fb = _ear_master_fb;
    if (fb == NULL) {
        _ear_clear_color[0] = r;
        _ear_clear_color[1] = g;
        _ear_clear_color[2] = b;
    } else {
        fb->desc.clear_color[0] = r;
        fb->desc.clear_color[1] = g;
        fb->desc.clear_color[2] = b;
        fb->desc.clear_color[3] = a;
    }
}
/*void
ear_clear(
    float r, float g, float b
    ) {
    eat_assert(_ear_cur_framebuffer != NULL, "cannot call clear on default framebuffer! use ear_clear_color instead!");

    _ear_vk_end_render_pass(_ear_vk_cur_frame);

    ear_vk_clear_framebuffer(_ear_cur_framebuffer->vk, 
        pow(r,2.2),
        pow(g,2.2),
        pow(b,2.2));

    ear_vk_bind_framebuffer(_ear_cur_framebuffer->vk);
}*/

void
ear_draw(
    uint32_t vertices, uint32_t instances
    ) {
    vkCmdDraw(_ear_vk_comm_buffers[_ear_vk_cur_frame], vertices, instances, 0,0);
}
void
ear_draw_idx(
    uint32_t indices, uint32_t instances
    ) {
    vkCmdDrawIndexed(_ear_vk_comm_buffers[_ear_vk_cur_frame], indices, instances, 0,0,0);
}

void
ear_mask(
    float x, float y, float w, float h
    ) {
    //ear_flush();

    /*if (_ear_cur_framebuffer == NULL) {
        gl.scissor(x,eaw_window_height - (y+h), w, h);
    } else {
        gl.scissor(x,_ear_cur_framebuffer->desc.height - (y+h), w, h);
    }*/
}
