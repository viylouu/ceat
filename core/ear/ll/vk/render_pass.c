#include "render_pass.h"
#include "../../../cutil.h"

#include "swapchain.h"
#include "device_log.h"

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
