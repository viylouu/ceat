#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../texture.h"

typedef struct ear_vk_texture ear_vk_texture;

ear_vk_texture*
ear_vk_create_texture(
    ear_texture_desc desc,
    uint8_t pixels[],
    uint32_t width, uint32_t height
    );
void
ear_vk_delete_texture(
    ear_vk_texture* tex
    );

struct ear_vk_texture{
    VkBuffer stagbuf;
    VkDeviceMemory stagmem;
    void* data;

    VkImage img;
    VkDeviceMemory imgmem;
    
    VkImageView imgview;
};
