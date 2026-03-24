#include "buffer.h"
#include "../../../../cutil.h"

#include "../init/device_log.h"
#include "../init/device_phys.h"
#include "../init/comm_pool.h"

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
    VkBuffer dst,
    VkDeviceSize size
    ) {
    VkCommandBufferAllocateInfo allocinfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,

        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,

        .commandPool        = _ear_vk_comm_pool,
        .commandBufferCount = 1,
        };

    VkCommandBuffer commbuf;
    vkAllocateCommandBuffers(_ear_vk_device, &allocinfo, &commbuf);

    VkCommandBufferBeginInfo begininfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,

        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,

        .pInheritanceInfo = NULL,
        };
    vkBeginCommandBuffer(commbuf, &begininfo);

    VkBufferCopy copyregion = {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = size,
        };
    vkCmdCopyBuffer(commbuf, src, dst, 1, &copyregion);

    vkEndCommandBuffer(commbuf);

    VkSubmitInfo submitinfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = NULL,

        .waitSemaphoreCount = 0,
        .pWaitSemaphores    = NULL,

        .pWaitDstStageMask = NULL,

        .commandBufferCount = 1,
        .pCommandBuffers    = &commbuf,

        .signalSemaphoreCount = 0,
        .pSignalSemaphores    = NULL,
        };

    vkQueueSubmit(_ear_vk_graphics_queue, 1, &submitinfo, NULL);
    vkQueueWaitIdle(_ear_vk_graphics_queue);

    vkFreeCommandBuffers(_ear_vk_device, _ear_vk_comm_pool, 1, &commbuf);
}

VkBufferUsageFlags
_ear_vk_convert_buf_type(
    ear_buffer_type type
    ) {
    VkBufferUsageFlags flags = VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    switch (type) {
    case EAR_BUF_VERTEX:  return flags | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    case EAR_BUF_UNIFORM: return flags | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    case EAR_BUF_STORAGE: return flags | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }

    eat_unreachable();
}
