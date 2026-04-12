#include "framebuffer.h"

#include <stdlib.h>
#include <math.h>

#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "../sc/render_pass.h"
#include "texture.h"
//#include "../util/texture.h"
#include "../util/macros.h"

bool _ear_vk_is_first_fb;

ear_vk_framebuffer* _ear_vk_last_fb;
bool _ear_vk_last_fb_depth;

void*
ear_vk_create_framebuffer(
    ear_framebuffer_desc* desc
    ) {
    ear_vk_framebuffer* fb = malloc(sizeof(ear_vk_framebuffer));
    fb->desc = desc;
    fb->frame = _ear_vk_cur_frame + 1;

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
        if (_ear_vk_last_fb == NULL) goto gtfo;

        if (_ear_vk_last_fb->desc->out_depth != NULL) {
            /*_ear_vk_trans_img_inplace(
                get_vk(texture, _ear_vk_last_fb->desc.out_depth)->img, true,
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                );*/
        }

        for (uint32_t i = 0; i < _ear_vk_last_fb->desc->out_color_amt; ++i)
            _ear_vk_trans_img(
                &get_vk(texture, _ear_vk_last_fb->desc->out_colors[i])->img, _ear_vk_comm_buffers[_ear_vk_cur_frame],
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                );
    }
gtfo:
    _ear_vk_is_first_fb = false;

    _ear_vk_last_fb = fb;
    _ear_vk_last_fb_depth = false;

    if (fb == NULL) _ear_vk_start_render_pass(_ear_vk_cur_img_index, _ear_vk_cur_frame);
    else {
        bool clear = fb->frame != _ear_vk_cur_frame;
        fb->frame = _ear_vk_cur_frame;

        VkRenderingAttachmentInfo depth_atch;
        if (fb->desc->out_depth != NULL) {
            _ear_vk_last_fb_depth = true;

            /*_ear_vk_trans_img_inplace(
                get_vk(texture, fb->desc->out_depth)->img, true,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                );*/

            depth_atch = (VkRenderingAttachmentInfo){
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .pNext = NULL,

                .imageView   = get_vk(texture, fb->desc->out_depth)->imgview,
                .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,

                .loadOp  = clear?
                    (fb->desc->clear? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD) :
                    VK_ATTACHMENT_LOAD_OP_LOAD,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

                .clearValue.depthStencil = (VkClearDepthStencilValue){1,0},
                };
        }

        VkRenderingAttachmentInfo* col_atchs = malloc(sizeof(VkRenderingAttachmentInfo) * fb->desc->out_color_amt);
        for (uint32_t i = 0; i < fb->desc->out_color_amt; ++i) {
            _ear_vk_trans_img(
                &get_vk(texture, fb->desc->out_colors[i])->img, _ear_vk_comm_buffers[_ear_vk_cur_frame],
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                );

            VkClearColorValue clearcol = {0};
            if (fb->desc->clear) { 
                clearcol = (VkClearColorValue){{
                    fb->desc->clear_color[0],
                    fb->desc->clear_color[1], 
                    fb->desc->clear_color[2], 
                    fb->desc->clear_color[3]
                    }};

                if (fb->desc->out_colors[i]->desc.type == EAR_TEX_COLOR) {
                    clearcol.float32[0] = pow(clearcol.float32[0], 2.2);
                    clearcol.float32[1] = pow(clearcol.float32[1], 2.2);
                    clearcol.float32[2] = pow(clearcol.float32[2], 2.2);
                }
            }

            col_atchs[i] = (VkRenderingAttachmentInfo){
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .pNext = NULL,

                .imageView   = get_vk(texture, fb->desc->out_colors[i])->imgview,
                .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

                .loadOp  = clear?
                    (fb->desc->clear? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD) :
                    VK_ATTACHMENT_LOAD_OP_LOAD,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,

                .clearValue.color = clearcol,
                };
        }

        VkRenderingInfo renderinfo = {
            .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
            .pNext = NULL,

            .renderArea.offset = {0,0},
            .renderArea.extent = {fb->desc->width, fb->desc->height},
            .layerCount = 1,

            .colorAttachmentCount = fb->desc->out_color_amt,
            .pColorAttachments    = col_atchs,

            .pDepthAttachment   = fb->desc->out_depth != NULL? &depth_atch : NULL,
            .pStencilAttachment = fb->desc->out_depth != NULL? &depth_atch : NULL,
            };

        vkCmdBeginRendering(_ear_vk_comm_buffers[_ear_vk_cur_frame], &renderinfo);
        _ear_vk_in_pass = true;

        free(col_atchs);
    }
}
