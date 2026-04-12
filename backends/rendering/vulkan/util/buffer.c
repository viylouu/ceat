#include "buffer.h"
#include "cutil.h"

#include "../init/device_log.h"
#include "../init/device_phys.h"
#include "../sc/swapchain.h"
#include "../vk.h"
#include "commbuf.h"
#include "../init/comm_buffer.h"

#include <string.h>

uint32_t 
_ear_vk_find_memory_type(
    uint32_t type_filter,
    VkMemoryPropertyFlags props
    ) {
    VkPhysicalDeviceMemoryProperties memprops;
    vkGetPhysicalDeviceMemoryProperties(_ear_vk_physical_device, &memprops);

    for (uint32_t i = 0; i < memprops.memoryTypeCount; ++i) if (type_filter & (1 << i) && (memprops.memoryTypes[i].propertyFlags & props) == props) return i;

    eat_error("failed to find suitable memory type for buffer!");
}

void
_ear_vk_make_buf(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags props,
    VkBuffer* obuf,
    VkDeviceMemory* omem
    ) {
    VkBufferCreateInfo bufferinfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .size = size,

        .usage = usage,

        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices   = NULL,
        };

    eat_assert(vkCreateBuffer(_ear_vk_device, &bufferinfo, NULL, obuf) == VK_SUCCESS,
        "failed to create buffer!");

    VkMemoryRequirements memreqs;
    vkGetBufferMemoryRequirements(_ear_vk_device, *obuf, &memreqs);

    VkMemoryAllocateInfo allocinfo = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = NULL,

        .allocationSize = memreqs.size,
        .memoryTypeIndex = _ear_vk_find_memory_type(
            memreqs.memoryTypeBits, 
            props
            ),
        };

    eat_assert(vkAllocateMemory(_ear_vk_device, &allocinfo, NULL, omem) == VK_SUCCESS,
        "failed to allocate buffer memory!");

    vkBindBufferMemory(_ear_vk_device, *obuf, *omem, 0);
}

void
_ear_vk_copy_buf(
    VkBuffer src,
    uint32_t srcoff,
    VkBuffer dst,
    uint32_t dstoff,
    VkDeviceSize size
    ) {
    VkCommandBuffer commbuf = _ear_vk_begin_stcomms();

    VkBufferCopy copyregion = {
        .srcOffset = srcoff,
        .dstOffset = dstoff,
        .size = size,
        };
    vkCmdCopyBuffer(commbuf, src, dst, 1, &copyregion);

    _ear_vk_end_stcomms(commbuf);
}
void
_ear_vk_copy_buf_implace(
    VkBuffer src,
    uint32_t srcoff,
    VkBuffer dst,
    uint32_t dstoff,
    VkDeviceSize size
    ) {
    VkBufferCopy copyregion = {
        .srcOffset = srcoff,
        .dstOffset = dstoff,
        .size = size,
        };
    vkCmdCopyBuffer(_ear_vk_comm_buffers[_ear_vk_cur_frame], src, dst, 1, &copyregion);
}


void
_ear_vk_make_buf_stage(
    ear_vk_buffer* buf,
    ear_buffer_desc desc,
    const void* data,
    uint32_t size
    ) {
    _ear_vk_make_buf(
        buf->chunk_bytes, _ear_vk_convert_buf_type(desc.type), 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &buf->gen.buffer, &buf->gen.memory
        );

    buf->gen.datas    = malloc(sizeof(void*)          * buf->insts);
    buf->gen.stagbufs = malloc(sizeof(VkBuffer)       * buf->insts);
    buf->gen.stagmems = malloc(sizeof(VkDeviceMemory) * buf->insts);

    for (uint32_t i = 0; i < buf->insts; ++i) {
        _ear_vk_make_buf(
            size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &buf->gen.stagbufs[i], &buf->gen.stagmems[i]
            );

        vkMapMemory(_ear_vk_device, buf->gen.stagmems[i], 0, buf->chunk_bytes, 0, &buf->gen.datas[i]);
        memcpy((uint8_t*)buf->gen.datas[i], data, buf->chunk_bytes);
    }

    _ear_vk_copy_buf(buf->gen.stagbufs[0], 0, buf->gen.buffer, 0, size);
}
void
_ear_vk_make_buf_pers(
    ear_vk_buffer* buf,
    ear_buffer_desc desc,
    const void* data,
    uint32_t size
    ) {
    buf->ubuf.has_sets = false;

    for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i) {
        _ear_vk_make_buf(
            size, _ear_vk_convert_buf_type(desc.type),
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &buf->ubuf.buffers[i], &buf->ubuf.memories[i]
            );

        vkMapMemory(_ear_vk_device, buf->ubuf.memories[i], 0, size, 0, &buf->ubuf.datas[i]);
        for (uint32_t j = 0; j < size/buf->chunk_bytes; ++j)
            memcpy((uint8_t*)buf->ubuf.datas[i] + j * buf->chunk_bytes, data, buf->chunk_bytes);
    }
}

void
_ear_vk_del_buf_stage(
    ear_vk_buffer* buf
    ) {
    for (uint32_t i = 0; i < buf->insts; ++i) {
        vkDestroyBuffer(_ear_vk_device, buf->gen.stagbufs[i], NULL);
        vkFreeMemory(_ear_vk_device, buf->gen.stagmems[i], NULL);
    }

    free(buf->gen.stagbufs);
    free(buf->gen.stagmems);
    free(buf->gen.datas);

    vkDestroyBuffer(_ear_vk_device, buf->gen.buffer, NULL);
    vkFreeMemory(_ear_vk_device, buf->gen.memory, NULL);
}
void
_ear_vk_del_buf_pers(
    ear_vk_buffer* buf
    ) {
    for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroyBuffer(_ear_vk_device, buf->ubuf.buffers[i], NULL);
        vkFreeMemory(_ear_vk_device, buf->ubuf.memories[i], NULL);
    }
}

void
_ear_vk_update_buf_pers(
    ear_vk_buffer* buf,
    const void* data,
    //uint32_t size,
    uint32_t off
    ) {
    memcpy((uint8_t*)buf->ubuf.datas[_ear_vk_cur_frame] + off * buf->chunk_bytes, data, buf->chunk_bytes);
}
void
_ear_vk_update_buf_stage(
    ear_vk_buffer* buf,
    const void* data,
    //uint32_t size,
    uint32_t off
    ) {
    uint32_t chunk_bytes = buf->chunk * buf->stride;
    if (chunk_bytes == 0) chunk_bytes = buf->size;

    memcpy(buf->gen.datas[off], data, chunk_bytes);

    _ear_vk_copy_buf(buf->gen.stagbufs[off], 0, buf->gen.buffer, 0, buf->chunk_bytes);
}

VkBufferUsageFlags
_ear_vk_convert_buf_type(
    ear_buffer_type type
    ) {
    VkBufferUsageFlags flags = VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    switch (type) {
    case EAR_BUF_VERTEX:             return flags | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    case EAR_BUF_UNIFORM: return flags | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    case EAR_BUF_STORAGE: return flags | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    case EAR_BUF_INDEX:              return flags | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }

    eat_unreachable();
}
VkShaderStageFlags
_ear_vk_convert_stage(
    ear_shader_stage stage
    ) {
    switch (stage) {
    case EAR_STAGE_VERTEX:   return VK_SHADER_STAGE_VERTEX_BIT;
    case EAR_STAGE_FRAGMENT: return VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    eat_unreachable();
}
