#include "render_pass.h"

#include <stdio.h>
#include <math.h>

#include "../init/comm_buffer.h"
#include "swapchain.h"
#include "image_views.h"
#include "../eng/screen.h"

bool _ear_vk_in_pass;
uint32_t last_rp_frame = 0;

void
_ear_vk_start_render_pass(
    uint32_t index,
    uint32_t frame
    ) {
    bool clear = last_rp_frame != frame;
    if (clear) last_rp_frame = frame;

    VkRenderingAttachmentInfo attach = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .pNext = NULL,

        .imageView   = _ear_vk_swapchain_img_views[index],
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

        .loadOp  = clear?
            VK_ATTACHMENT_LOAD_OP_CLEAR :
            VK_ATTACHMENT_LOAD_OP_LOAD,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

        .clearValue.color = {{
            _ear_clear_color[0],
            _ear_clear_color[1], 
            _ear_clear_color[2], 
            1}},
        };

    if (_ear_vk_swapchain_img_fmt == VK_FORMAT_B8G8R8A8_SRGB) {
        attach.clearValue.color.float32[0] = pow(_ear_clear_color[0], 2.2);
        attach.clearValue.color.float32[1] = pow(_ear_clear_color[1], 2.2);
        attach.clearValue.color.float32[2] = pow(_ear_clear_color[2], 2.2);
    }

    VkRenderingInfo renderinfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .pNext = NULL,

        .renderArea.offset = {0,0},
        .renderArea.extent = _ear_vk_swapchain_extent,
        .layerCount = 1,

        .colorAttachmentCount = 1,
        .pColorAttachments    = &attach,

        .pDepthAttachment = NULL,
        };

    vkCmdBeginRendering(_ear_vk_comm_buffers[frame], &renderinfo);

    _ear_vk_in_pass = true;
}
void
_ear_vk_end_render_pass(
    uint32_t frame
    ) {
    if (!_ear_vk_in_pass) return;
    vkCmdEndRendering(_ear_vk_comm_buffers[frame]);
    _ear_vk_in_pass = false;
}
