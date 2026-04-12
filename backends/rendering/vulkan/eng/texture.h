#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ear/ll/texture.h"
#include "../util/img.h"

typedef struct ear_vk_texture ear_vk_texture;

void*
ear_vk_create_texture(
    ear_texture_desc desc,
    uint8_t** pixels,
    uint32_t width, uint32_t height
    );
void
ear_vk_delete_texture(
    void* tex
    );

void
ear_vk_update_texture(
    void* tex,
    const uint8_t* pixels,
    uint32_t width, uint32_t height
    );

struct ear_vk_texture{
    VkBuffer stagbuf;
    VkDeviceMemory stagmem;
    void* data;

    _ear_vk_image img;
    VkDeviceMemory imgmem;
    
    VkImageView imgview;

    VkSampler samp;

    VkFormat format;
    ear_texture_type type;

    uint32_t width; uint32_t height;
    bool aniso;
};
