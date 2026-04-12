#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <stdbool.h>

typedef struct _ear_vk_image{
    VkImage img;
    VkImageLayout lay;
    bool depth;
} _ear_vk_image;

void
_ear_vk_trans_img(
    _ear_vk_image* img,
    VkCommandBuffer buf,
    VkImageLayout newlay
    );

VkAccessFlags
_ear_vk_convert_lay_access(
    VkImageLayout lay
    );
VkPipelineStageFlags
_ear_vk_convert_lay_stage(
    VkImageLayout lay
    );
