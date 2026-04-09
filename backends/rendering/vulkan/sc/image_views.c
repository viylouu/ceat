#include "image_views.h"

#include <stdlib.h>

#include "swapchain.h"
#include "../init/device_log.h"
#include "../util/texture.h"

VkImageView* _ear_vk_swapchain_img_views;

void
_ear_vk_create_image_views(
    void
    ) {
    _ear_vk_swapchain_img_views = malloc(sizeof(VkImageView) * _ear_vk_swapchain_img_amt);

    for (uint32_t i = 0; i < _ear_vk_swapchain_img_amt; ++i)
        _ear_vk_make_imgview(
            _ear_vk_swapchain_imgs[i], _ear_vk_swapchain_img_fmt,
            &_ear_vk_swapchain_img_views[i]
            );
}
void
_ear_vk_delete_image_views(
    void
    ) {
    for (uint32_t i = 0; i < _ear_vk_swapchain_img_amt; ++i)
        vkDestroyImageView(_ear_vk_device, _ear_vk_swapchain_img_views[i], NULL);
    free(_ear_vk_swapchain_img_views);
}
