#include "framebuffer.h"
//#include "../../../../cutil.h"

#include <stdlib.h>
#include <math.h>

#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "../sc/render_pass.h"
#include "texture.h"
#include "../util/texture.h"
//#include "../init/device_log.h"
#include "../util/macros.h"

bool _ear_vk_is_first_fb;

ear_vk_framebuffer* last_fb;

void*
ear_vk_create_framebuffer(
    ear_framebuffer_desc desc
    ) {
    ear_vk_framebuffer* fb = malloc(sizeof(ear_vk_framebuffer));
    fb->desc = desc;

    return fb;
}
void
ear_vk_delete_framebuffer(
    void* fb
    ) {
    free(fb);
}

void
ear_vk_bind_framebuffer(
    void* _fb
    ) {
    ear_vk_framebuffer* fb = _fb;

    if (!_ear_vk_is_first_fb) {
        _ear_vk_end_render_pass(_ear_vk_cur_frame);
        if (last_fb == NULL) goto gtfo;

        if (last_fb->desc.out_depth != NULL) {
            _ear_vk_trans_img_inplace(
                get_vk(texture, last_fb->desc.out_depth)->img, true,
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                );
        }

        for (uint32_t i = 0; i < last_fb->desc.out_color_amt; ++i)
            _ear_vk_trans_img_inplace(
                get_vk(texture, last_fb->desc.out_colors[i])->img, false,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                );
    }
gtfo:
    _ear_vk_is_first_fb = false;

    last_fb = fb;

    if (fb == NULL) _ear_vk_start_render_pass(_ear_vk_cur_img_index, _ear_vk_cur_frame);
    else {
        VkRenderingAttachmentInfo depth_atch;
        if (fb->desc.out_depth != NULL) {
            /*_ear_vk_trans_img_inplace(
                get_vk(texture, fb->desc.out_depth)->img, true,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                );*/

            depth_atch = (VkRenderingAttachmentInfo){
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .pNext = NULL,

                .imageView   = get_vk(texture, fb->desc.out_depth)->imgview,
                .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,

                .loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

                .clearValue.depthStencil = (VkClearDepthStencilValue){1,0},
                };
        }

        VkRenderingAttachmentInfo* col_atchs = malloc(sizeof(VkRenderingAttachmentInfo) * fb->desc.out_color_amt);
        for (uint32_t i = 0; i < fb->desc.out_color_amt; ++i) {
            _ear_vk_trans_img_inplace(
                get_vk(texture, fb->desc.out_colors[i])->img, false,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                );

            VkClearColorValue clearcol = {0};
            if (fb->desc.clear) { 
                clearcol = (VkClearColorValue){{
                    fb->desc.clear_color[0],
                    fb->desc.clear_color[1], 
                    fb->desc.clear_color[2], 
                    fb->desc.clear_color[3]
                    }};

                if (fb->desc.out_colors[i]->desc.type == EAR_TEX_COLOR) {
                    clearcol.float32[0] = pow(clearcol.float32[0], 2.2);
                    clearcol.float32[1] = pow(clearcol.float32[1], 2.2);
                    clearcol.float32[2] = pow(clearcol.float32[2], 2.2);
                }
            }

            col_atchs[i] = (VkRenderingAttachmentInfo){
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .pNext = NULL,

                .imageView   = get_vk(texture, fb->desc.out_colors[i])->imgview,
                .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

                .loadOp  = fb->desc.clear? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

                .clearValue.color = clearcol,
                };
        }

        VkRenderingInfo renderinfo = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
            .pNext = NULL,

            .renderArea.offset = {0,0},
            .renderArea.extent = {fb->desc.width, fb->desc.height},
            .layerCount = 1,

            .colorAttachmentCount = fb->desc.out_color_amt,
            .pColorAttachments    = col_atchs,

            .pDepthAttachment   = fb->desc.out_depth != NULL? &depth_atch : NULL,
            .pStencilAttachment = fb->desc.out_depth != NULL? &depth_atch : NULL,
            };

        vkCmdBeginRendering(_ear_vk_comm_buffers[_ear_vk_cur_frame], &renderinfo);

        free(col_atchs);
    }
}
