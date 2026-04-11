#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ear/ll/buffer.h"
#include "bindset.h"
#include "../vk.h"

typedef struct ear_vk_buffer ear_vk_buffer;

void*
ear_vk_create_buffer(
    ear_buffer_desc desc,
    const void* data,
    uint32_t size
    );
void
ear_vk_delete_buffer(
    void* buf
    );

void
ear_vk_bind_buffer(
    void* buf,
    uint32_t slot,
    uint32_t offset
    );

void
ear_vk_update_buffer(
    void* buf,
    const void* data,
    //uint32_t size,
    uint32_t offset
    );

struct ear_vk_buffer{
    uint32_t size;
    uint32_t stride;
    uint32_t chunk;
    uint32_t insts;
    uint32_t chunk_bytes;

    ear_vk_bindset* cur_set;

    ear_buffer_type type;
    struct{
        VkBuffer buffer;
        VkDeviceMemory memory;

        VkBuffer* stagbufs;
        VkDeviceMemory* stagmems;
        void** datas;
        } gen;
    struct{
        VkBuffer buffers[EAR_VK_MAX_FRAMES_IN_FLIGHT];
        VkDeviceMemory memories[EAR_VK_MAX_FRAMES_IN_FLIGHT];
        void* datas[EAR_VK_MAX_FRAMES_IN_FLIGHT];

        bool has_sets;
        } ubuf;
};
