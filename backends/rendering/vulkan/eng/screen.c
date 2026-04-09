#include "screen.h"

#include "../sc/swapchain.h"
#include "../init/comm_buffer.h"

#include <math.h>

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
