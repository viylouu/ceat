#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <stdbool.h>

#include "ear/ll/texture.h"

void
_ear_vk_make_img(
    uint32_t width, uint32_t height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags props,
    VkImage* img, VkDeviceMemory* mem
    );
void
_ear_vk_trans_img(
    VkImage img, bool depth,
    VkImageLayout oldlay,
    VkImageLayout newlay
    );
void
_ear_vk_trans_img_inplace(
    VkImage img, bool depth,
    VkImageLayout oldlay,
    VkImageLayout newlay
    );
void
_ear_vk_copy_buf_img(
    VkBuffer buf, VkImage img,
    uint32_t width, uint32_t height,
    bool depth
    );
void
_ear_vk_make_imgview(
    VkImage image, VkFormat format,
    bool depth,
    VkImageView* view
    );
void
_ear_vk_make_sampler(
    bool anisotropy,
    VkSampler* sampler
    );

VkAccessFlags
_ear_vk_convert_lay_access(
    VkImageLayout lay
    );
VkPipelineStageFlags
_ear_vk_convert_lay_stage(
    VkImageLayout lay
    );
VkFormat
_ear_vk_convert_tex_fmt(
    ear_texture_type type
    );
