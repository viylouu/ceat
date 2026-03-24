#include "buffer.h"
#include "../../../../cutil.h"

#include <string.h>

#include "../init/device_log.h"
#include "../init/device_phys.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"

VkBufferUsageFlags
_ear_vk_convert_buf_type(
    ear_buffer_type type
    ) {
    switch (type) {
    case EAR_BUF_VERTEX:  return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    case EAR_BUF_UNIFORM: return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    case EAR_BUF_STORAGE: return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }

    eat_unreachable();
}

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

ear_vk_buffer*
ear_vk_create_buffer(
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    ) {
    ear_vk_buffer* buf = malloc(sizeof(ear_vk_buffer));
    buf->type = desc.type;

    VkBufferCreateInfo bufferinfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .size = size,

        .usage = _ear_vk_convert_buf_type(desc.type),

        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices   = NULL,
        };

    eat_assert(vkCreateBuffer(_ear_vk_device, &bufferinfo, NULL, &buf->buffer) == VK_SUCCESS,
        "failed to create buffer!");

    VkMemoryRequirements memreqs;
    vkGetBufferMemoryRequirements(_ear_vk_device, buf->buffer, &memreqs);

    VkMemoryAllocateInfo allocinfo = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = NULL,

        .allocationSize = memreqs.size,
        .memoryTypeIndex = _ear_vk_find_memory_type(
            memreqs.memoryTypeBits, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            ),
        };

    eat_assert(vkAllocateMemory(_ear_vk_device, &allocinfo, NULL, &buf->memory) == VK_SUCCESS,
        "failed to allocate buffer memory!");

    vkBindBufferMemory(_ear_vk_device, buf->buffer, buf->memory, 0);

    vkMapMemory(_ear_vk_device, buf->memory, 0, bufferinfo.size, 0, &buf->data);
    memcpy(buf->data, data, bufferinfo.size);
    vkUnmapMemory(_ear_vk_device, buf->memory);

    return buf;
}
void
ear_vk_delete_buffer(
    ear_vk_buffer* buf
    ) {
    vkDestroyBuffer(_ear_vk_device, buf->buffer, NULL);
    vkFreeMemory(_ear_vk_device, buf->memory, NULL);

    free(buf);
}

void
ear_vk_bind_buffer(
    ear_vk_buffer* buf,
    uint32_t slot
    ) {
    switch (buf->type) {
    case EAR_BUF_VERTEX:
        vkCmdBindVertexBuffers(_ear_vk_comm_buffers[_ear_vk_cur_frame], slot, 1, &buf->buffer, &(VkDeviceSize){0});
        break;
    }
}
