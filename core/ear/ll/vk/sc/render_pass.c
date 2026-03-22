#include "render_pass.h"
#include "../../../../cutil.h"

#include "../init/comm_buffer.h"
#include "framebuffer.h"
#include "swapchain.h"
#include "../init/device_log.h"

VkRenderPass _ear_vk_renderpass;

void
_ear_vk_create_render_pass(
    void
    ) {
    VkAttachmentDescription colorattach = {
        .format  = _ear_vk_swapchain_img_fmt,
        .samples = VK_SAMPLE_COUNT_1_BIT,

        .loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

        .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        };

    VkAttachmentReference colorattachref = {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,

        .colorAttachmentCount = 1,
        .pColorAttachments    = &colorattachref,
        };

    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,

        .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,

        .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        };

    VkRenderPassCreateInfo renderpassinfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .attachmentCount = 1,
        .pAttachments    = &colorattach,

        .subpassCount = 1,
        .pSubpasses   = &subpass,

        .dependencyCount = 1,
        .pDependencies   = &dependency,
        };

    eat_assert(vkCreateRenderPass(_ear_vk_device, &renderpassinfo, NULL, &_ear_vk_renderpass) == VK_SUCCESS, 
        "failed to create render pass!");
}
void
_ear_vk_delete_render_pass(
    void
    ) {
    vkDestroyRenderPass(_ear_vk_device, _ear_vk_renderpass, NULL);
}

void
_ear_vk_start_render_pass(
    uint32_t index,
    uint32_t frame
    ) {
    VkClearValue clearvalues[2];

    clearvalues[0].color = (VkClearColorValue){{ 0,0,0,1 }};
    clearvalues[1].depthStencil = (VkClearDepthStencilValue){1,0};

    VkRenderPassBeginInfo renderpassinfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = NULL,

        .renderPass  = _ear_vk_renderpass,
        .framebuffer = _ear_vk_swapchain_fbufs[index],

        .renderArea.offset = {0,0},
        .renderArea.extent = _ear_vk_swapchain_extent,

        .clearValueCount = 2,
        .pClearValues    = clearvalues,
        };

    vkCmdBeginRenderPass(_ear_vk_comm_buffers[frame], &renderpassinfo, VK_SUBPASS_CONTENTS_INLINE);
}
void
_ear_vk_end_render_pass(
    uint32_t frame
    ) {
    vkCmdEndRenderPass(_ear_vk_comm_buffers[frame]);
}
