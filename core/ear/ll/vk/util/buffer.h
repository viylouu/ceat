#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../buffer.h"

uint32_t 
_ear_vk_find_memory_type(
    uint32_t type_filter,
    VkMemoryPropertyFlags props
    );

void
_ear_vk_make_buf(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags props,
    VkBuffer* obuf,
    VkDeviceMemory* omem
    );

void
_ear_vk_copy_buf(
    VkBuffer src,
    VkBuffer dst,
    VkDeviceSize size
    );

VkBufferUsageFlags
_ear_vk_convert_buf_type(
    ear_buffer_type type
    );
