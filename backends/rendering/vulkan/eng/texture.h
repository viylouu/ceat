#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ear/ll/texture.h"

typedef struct ear_vk_texture ear_vk_texture;

ear_vk_texture*
ear_vk_create_texture(
    ear_texture_desc desc,
    uint8_t** pixels,
    uint32_t width, uint32_t height
    );
void
ear_vk_delete_texture(
    ear_vk_texture* tex
    );

void
ear_vk_update_texture(
    ear_vk_texture* tex,
    ear_texture_type type,
    uint8_t pixels[],
    uint32_t width, uint32_t height
    );

struct ear_vk_texture{
    VkBuffer stagbuf;
    VkDeviceMemory stagmem;
    void* data;

    // image layout should be SHADER_READ_ONLY_OPTIMAL between calls
    VkImage img;
    VkDeviceMemory imgmem;
    
    VkImageView imgview;

    VkSampler samp;

    VkFormat format;
};
