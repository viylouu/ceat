#include "framebuffer.h"
#include "../../../../cutil.h"

#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "../sc/render_pass.h"
#include "texture.h"
#include "../util/texture.h"
#include "../init/device_log.h"

bool _ear_vk_is_first_fb;

ear_vk_framebuffer*
ear_vk_create_framebuffer(
    ear_framebuffer_desc desc
    ) {
    ear_vk_framebuffer* fb = malloc(sizeof(ear_vk_framebuffer));
    fb->desc = desc;

    uint32_t atch_amt = desc.out_color_amt + (desc.out_depth != NULL);
    VkAttachmentDescription* attaches = malloc(sizeof(VkAttachmentDescription) * atch_amt);

    for (uint32_t i = 0; i < desc.out_color_amt; ++i)
        attaches[i] = (VkAttachmentDescription){
            .flags = 0,

            .format = ((ear_vk_texture*)desc.out_colors[i]->vk)->format,

            .samples = VK_SAMPLE_COUNT_1_BIT,

            .loadOp  = desc.clear? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

            .initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            };
    if (desc.out_depth != NULL) {
        attaches[desc.out_color_amt]  = (VkAttachmentDescription){
            .flags = 0,

            .format = ((ear_vk_texture*)desc.out_depth->vk)->format,

            .samples = VK_SAMPLE_COUNT_1_BIT,

            .loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

            .initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            };
    }

    VkAttachmentReference* col_refs = malloc(sizeof(VkAttachmentReference) * desc.out_color_amt);
    for (uint32_t i = 0; i < desc.out_color_amt; ++i)
        col_refs[i] = (VkAttachmentReference){
            .attachment = i,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            };
    VkAttachmentReference depth_ref = {
        .attachment = desc.out_color_amt,
        .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,

        .colorAttachmentCount = desc.out_color_amt,
        .pColorAttachments    = col_refs,

        .pDepthStencilAttachment = desc.out_depth == NULL? NULL : &depth_ref,
        };

    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,

        .srcStageMask  = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,

        .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                         VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
        };

    if (desc.out_depth != NULL) {
        dependency.dstStageMask  |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
                                    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
    }

    VkRenderPassCreateInfo renderpassinfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .attachmentCount = atch_amt,
        .pAttachments    = attaches,

        .subpassCount = 1,
        .pSubpasses   = &subpass,

        .dependencyCount = 1,
        .pDependencies   = &dependency,
        };

    eat_assert(vkCreateRenderPass(_ear_vk_device, &renderpassinfo, NULL, &fb->pass) == VK_SUCCESS, 
        "failed to create render pass!");

    free(col_refs);
    free(attaches);

    uint32_t view_amt = desc.out_color_amt + (desc.out_depth != NULL);
    VkImageView* views = malloc(sizeof(VkImageView) * view_amt);
    for (uint32_t i = 0; i < desc.out_color_amt; ++i)
        views[i] = ((ear_vk_texture*)desc.out_colors[i]->vk)->imgview;
    if (desc.out_depth != NULL)
        views[desc.out_color_amt] = ((ear_vk_texture*)desc.out_depth->vk)->imgview;

    VkFramebufferCreateInfo createinfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .renderPass = fb->pass,

        .attachmentCount = view_amt,
        .pAttachments    = views,

        .width  = desc.width,
        .height = desc.height,
        .layers = 1,
        };

    eat_assert(vkCreateFramebuffer(_ear_vk_device, &createinfo, NULL, &fb->fb) == VK_SUCCESS,
        "failed to create framebuffer!");

    free(views);

    return fb;
}
void
ear_vk_delete_framebuffer(
    ear_vk_framebuffer* fb
    ) {
    vkDestroyFramebuffer(_ear_vk_device, fb->fb, NULL);
    vkDestroyRenderPass(_ear_vk_device, fb->pass, NULL);

    free(fb);
}

void
ear_vk_bind_framebuffer(
    ear_vk_framebuffer* fb
    ) {
    if (!_ear_vk_is_first_fb) _ear_vk_end_render_pass(_ear_vk_cur_frame);    
    _ear_vk_is_first_fb = false;

    if (fb == NULL) _ear_vk_start_render_pass(_ear_vk_cur_img_index, _ear_vk_cur_frame);
    else {
        VkClearValue depth_clear;
        depth_clear.depthStencil = (VkClearDepthStencilValue){1,0};

        VkRenderPassBeginInfo renderpassinfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = NULL,

            .renderPass  = fb->pass,
            .framebuffer = fb->fb,

            .renderArea.offset = {0,0},
            .renderArea.extent = { fb->desc.width, fb->desc.height },

            .clearValueCount = fb->desc.out_depth != NULL? 1            : 0,
            .pClearValues    = fb->desc.out_depth != NULL? &depth_clear : NULL,
            };

        VkClearValue* clearvals;
        if (fb->desc.clear) {
            uint32_t clears = fb->desc.out_color_amt + (fb->desc.out_depth != NULL);
            clearvals = malloc(sizeof(VkClearValue) * clears);
            for (uint32_t i = 0; i < fb->desc.out_color_amt; ++i)
                clearvals[i].color = (VkClearColorValue){{
                    fb->desc.clear_color[0],
                    fb->desc.clear_color[1], 
                    fb->desc.clear_color[2], 
                    fb->desc.clear_color[3],
                    }};
            if (fb->desc.out_depth != NULL) clearvals[fb->desc.out_color_amt] = depth_clear;

            renderpassinfo.clearValueCount = clears;
            renderpassinfo.pClearValues    = clearvals;
        }

        vkCmdBeginRenderPass(_ear_vk_comm_buffers[_ear_vk_cur_frame], &renderpassinfo, VK_SUBPASS_CONTENTS_INLINE);

        if (fb->desc.clear) free(clearvals);
    }
}
