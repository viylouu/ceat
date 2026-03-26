#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../buffer.h"
#include "../eng/buffer.h"

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

void
_ear_vk_make_buf_vi(
    ear_vk_buffer* buf,
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    );
void
_ear_vk_make_buf_u(
    ear_vk_buffer* buf,
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    );
void
_ear_vk_make_buf_u_set(
    ear_vk_buffer* buf,
    ear_buffer_bind_set set
    );

void
_ear_vk_update_buf_u(
    ear_vk_buffer* buf,
    void* data,
    uint32_t size
    );

VkBufferUsageFlags
_ear_vk_convert_buf_type(
    ear_buffer_type type
    );
VkDescriptorType
_ear_vk_convert_desc_type(
    ear_buffer_type type
    );
VkDescriptorSetLayout
_ear_vk_convert_bind_set(
    ear_buffer_bind_set set
    );
