#include "framebuffer.h"
#include "../../../../cutil.h"

#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "texture.h"
#include "../util/texture.h"

ear_vk_framebuffer*
ear_vk_create_framebuffer(
    ear_framebuffer_desc desc
    ) {
    ear_vk_framebuffer* fb = malloc(sizeof(ear_vk_framebuffer));

    return fb;
}
void
ear_vk_delete_framebuffer(
    ear_vk_framebuffer* fb
    ) {
    free(fb);
}

void
ear_vk_clear_framebuffer(
    ear_vk_framebuffer* fb,
    float r, float g, float b
    ) {
    uint32_t imgcount = fb->desc.out_color_amt + fb->desc.out_depth != NULL;
    VkImage* imgs = malloc(sizeof(VkImage) * imgcount);
    for (uint32_t i = 0; i < fb->desc.out_color_amt; ++i)
        imgs[i] = ((ear_vk_texture*)fb->desc.out_colors[i]->vk)->img;
    if (fb->desc.out_depth != NULL)
        imgs[fb->desc.out_color_amt] = ((ear_vk_texture*)fb->desc.out_depth->vk)->img;

    for (uint32_t i = 0; i < imgcount; ++i) {
        _ear_vk_trans_img(
            imgs[i],
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
            );

        VkClearColorValue color = {{ r,g,b, 1 }};

        VkImageSubresourceRange range = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseArrayLayer = 0,
            .layerCount     = 1,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            };

        vkCmdClearColorImage(
            _ear_vk_comm_buffers[_ear_vk_cur_frame],
            imgs[i],
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            &color,
            1,
            &range
            );

        _ear_vk_trans_img(
            imgs[i],
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            );
    }

    free(imgs);
}

void
ear_vk_bind_framebuffer(
    ear_vk_framebuffer* fb
    ) {

}
