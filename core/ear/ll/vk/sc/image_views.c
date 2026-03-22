#include "image_views.h"
#include "../../../../cutil.h"

#include "swapchain.h"
#include "../init/device_log.h"

VkImageView* _ear_vk_swapchain_img_views;

void
_ear_vk_create_image_views(
    void
    ) {
    _ear_vk_swapchain_img_views = malloc(sizeof(VkImageView) * _ear_vk_swapchain_img_amt);

    for (uint32_t i = 0; i < _ear_vk_swapchain_img_amt; ++i) {
        VkImageViewCreateInfo createinfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .image = _ear_vk_swapchain_imgs[i],

            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = _ear_vk_swapchain_img_fmt,

            .components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .components.a = VK_COMPONENT_SWIZZLE_IDENTITY,

            .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .subresourceRange.baseMipLevel   = 0,
            .subresourceRange.levelCount     = 1,
            .subresourceRange.baseArrayLayer = 0,
            .subresourceRange.layerCount     = 1,
            };

        eat_assert(vkCreateImageView(_ear_vk_device, &createinfo, NULL, &_ear_vk_swapchain_img_views[i]) == VK_SUCCESS,
            "failed to create image views!");
    }
}
void
_ear_vk_delete_image_views(
    void
    ) {
    for (uint32_t i = 0; i < _ear_vk_swapchain_img_amt; ++i)
        vkDestroyImageView(_ear_vk_device, _ear_vk_swapchain_img_views[i], NULL);
    free(_ear_vk_swapchain_img_views);
}
