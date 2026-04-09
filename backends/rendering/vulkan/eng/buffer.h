#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ear/ll/buffer.h"
//#include "../../bindset.h"
#include "bindset.h"
#include "../vk.h"

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
    uint32_t slot,
    uint32_t offset
    );

void
ear_vk_update_buffer(
    ear_vk_buffer* buf,
    void* data,
    //uint32_t size,
    uint32_t offset
    );

struct ear_vk_buffer{
    uint32_t size;
    uint32_t stride;
    uint32_t chunk;

    ear_vk_bindset* cur_set;

    ear_buffer_type type;
    struct{
        VkBuffer buffer;
        VkDeviceMemory memory;
 
        void* data;
        } gen;
    struct{
        VkBuffer buffers[EAR_VK_MAX_FRAMES_IN_FLIGHT];
        VkDeviceMemory memories[EAR_VK_MAX_FRAMES_IN_FLIGHT];
        void* datas[EAR_VK_MAX_FRAMES_IN_FLIGHT];

        bool has_sets;
        } ubuf;
};
