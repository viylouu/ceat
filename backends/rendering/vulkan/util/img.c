#include "img.h"
#include "cutil.h"

#include "../init/comm_buffer.h"

void
_ear_vk_trans_img(
    _ear_vk_image* img,
    VkCommandBuffer buf,
    VkImageLayout newlay
    ) {
    if (img->lay == newlay) return;

    eat_assert(_ear_vk_commbuf_rec, "command buffer is not recording! cannot trans img!");

    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = NULL,

        .oldLayout = img->lay,
        .newLayout = newlay,

        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,

        .image = img->img,

        .subresourceRange.aspectMask     = img->depth? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT : 
                                                  VK_IMAGE_ASPECT_COLOR_BIT,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1,

        .srcAccessMask = _ear_vk_convert_lay_access(img->lay),
        .dstAccessMask = _ear_vk_convert_lay_access(newlay),
        };

    vkCmdPipelineBarrier(
        buf,
        _ear_vk_convert_lay_stage(img->lay),
        _ear_vk_convert_lay_stage(newlay),
        0,
        0, NULL,
        0, NULL,
        1, &barrier
        );

    img->lay = newlay;
}

VkAccessFlags
_ear_vk_convert_lay_access(
    VkImageLayout lay
    ) {
    switch (lay) {
    case VK_IMAGE_LAYOUT_UNDEFINED:                        return 0;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:             return VK_ACCESS_TRANSFER_WRITE_BIT;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:         return VK_ACCESS_SHADER_READ_BIT;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:         return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                                                                  VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:                  return 0;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                                                                  VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    default: eat_unreachable();
    }

    eat_unreachable();
}
VkPipelineStageFlags
_ear_vk_convert_lay_stage(
    VkImageLayout lay
    ) {
    switch (lay) {
    case VK_IMAGE_LAYOUT_UNDEFINED:                        return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:             return VK_PIPELINE_STAGE_TRANSFER_BIT;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:         return VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:         return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:                  return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    default: eat_unreachable();
    }

    eat_unreachable();
}
