#pragma once

#include <stdint.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

extern VkImageView* _ear_vk_swapchain_img_views;

void
_ear_vk_create_image_views(
    void
    );
void
_ear_vk_delete_image_views(
    void
    );
