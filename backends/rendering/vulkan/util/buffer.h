#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ear/ll/buffer.h"
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
    uint32_t srcoff,
    VkBuffer dst,
    uint32_t dstoff,
    VkDeviceSize size
    );

void
_ear_vk_make_buf_stage(
    ear_vk_buffer* buf,
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    );
void
_ear_vk_make_buf_pers(
    ear_vk_buffer* buf,
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    );

void
_ear_vk_del_buf_stage(
    ear_vk_buffer* buf
    );
void
_ear_vk_del_buf_pers(
    ear_vk_buffer* buf
    );

void
_ear_vk_update_buf_pers(
    ear_vk_buffer* buf,
    const void* data,
    //uint32_t size,
    uint32_t off
    );
void
_ear_vk_update_buf_stage(
    ear_vk_buffer* buf,
    const void* data,
    uint32_t size,
    uint32_t off
    );

VkBufferUsageFlags
_ear_vk_convert_buf_type(
    ear_buffer_type type
    );
VkShaderStageFlags
_ear_vk_convert_stage(
    ear_shader_stage stage
    );
//VkDescriptorSetLayout
//_ear_vk_convert_bindset(
//    ear_bindset* set
//    );
