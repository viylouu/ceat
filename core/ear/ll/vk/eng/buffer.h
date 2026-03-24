#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../buffer.h"

typedef struct ear_vk_buffer ear_vk_buffer;

ear_vk_buffer*
ear_vk_create_buffer(
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    );
void
ear_vk_delete_buffer(
    ear_vk_buffer* buf
    );

void
ear_vk_bind_buffer(
    ear_vk_buffer* buf,
    uint32_t slot
    );

struct ear_vk_buffer{
    VkBuffer buffer;
    VkDeviceMemory memory;
    void* data;
    ear_buffer_type type;
};
