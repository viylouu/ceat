#include "misc.h"
#include "../../cutil.h"

#include "../../eaw/window.h"
//#include "user.h"
//#include "framebuffer.h"

#include "vk/sc/swapchain.h"
#include "vk/init/comm_buffer.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>


void 
ear_clear(
    float r, float g, float b, float a
    ) {
    //gl.clearColor(col[0], col[1], col[2], 1);
    //gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
ear_draw(
    int32_t vertices, int32_t instances
    ) {
    vkCmdDraw(_ear_vk_comm_buffers[_ear_vk_cur_frame], vertices, instances, 0,0);
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
