#include "screen.h"

#include "../sc/swapchain.h"
#include "../init/comm_buffer.h"
#include "framebuffer.h"

#include <math.h>

float _ear_clear_color[3] = {0};

void
ear_vk_set_viewport(
    float x, float y,
    float w, float h
    ) {
    VkViewport viewport = {
        .x = fmax(x, 0),
        .y = fmax(y, 0),
        .width  = fmin(w + x, _ear_vk_swapchain_extent.width)  - x,
        .height = fmin(h + y, _ear_vk_swapchain_extent.height) - y,

        .minDepth = 0,
        .maxDepth = 1,
        };
    vkCmdSetViewport(_ear_vk_comm_buffers[_ear_vk_cur_frame], 0,1, &viewport);
}
void
ear_vk_set_scissor(
    float x, float y,
    float w, float h
    ) {
    VkRect2D scissor = {
        .offset = { fmax(x,0), fmax(y,0) },
        .extent = {
            fmin(w + x, _ear_vk_swapchain_extent.width)  - x,
            fmin(h + y, _ear_vk_swapchain_extent.height) - y,
            },
        };
    vkCmdSetScissor(_ear_vk_comm_buffers[_ear_vk_cur_frame], 0,1, &scissor);
}

void
ear_vk_draw(
    uint32_t vertices, uint32_t instances
    ) {
    vkCmdDraw(_ear_vk_comm_buffers[_ear_vk_cur_frame], vertices, instances, 0,0);
}
void
ear_vk_draw_idx(
    uint32_t indices, uint32_t instances
    ) {
    vkCmdDrawIndexed(_ear_vk_comm_buffers[_ear_vk_cur_frame], indices, instances, 0,0,0);
}

void
ear_vk_clear(
    void* _fb,
    float r, float g, float b, float a
    ) {
    if (_fb == NULL) {
        _ear_clear_color[0] = r;
        _ear_clear_color[1] = g;
        _ear_clear_color[2] = b;
    } else {
        ear_vk_framebuffer* fb = _fb;
        fb->desc.clear_color[0] = r;
        fb->desc.clear_color[1] = g;
        fb->desc.clear_color[2] = b;
        fb->desc.clear_color[3] = a;
    }
}
