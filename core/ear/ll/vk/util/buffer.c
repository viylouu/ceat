#include "buffer.h"
#include "../../../../cutil.h"

#include "../init/device_log.h"
#include "../init/device_phys.h"
#include "../init/comm_pool.h"
#include "../sc/swapchain.h"
#include "../vk.h"

#include "../../buffer.h"

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

void
_ear_vk_make_buf_vi(
    ear_vk_buffer* buf,
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    ) {
    VkBuffer stagbuf;
    VkDeviceMemory stagmem;
    _ear_vk_make_buf(
        size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagbuf, &stagmem
        );

    vkMapMemory(_ear_vk_device, stagmem, 0, size, 0, &buf->gen.data);
    memcpy(buf->gen.data, data, size);
    vkUnmapMemory(_ear_vk_device, stagmem);

    _ear_vk_make_buf(
        size, _ear_vk_convert_buf_type(desc.type), 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &buf->gen.buffer, &buf->gen.memory
        );

    _ear_vk_copy_buf(stagbuf, buf->gen.buffer, size);

    vkDestroyBuffer(_ear_vk_device, stagbuf, NULL);
    vkFreeMemory(_ear_vk_device, stagmem, NULL);
}
void
_ear_vk_make_buf_u(
    ear_vk_buffer* buf,
    ear_buffer_desc desc,
    void* data,
    uint32_t size
    ) {
    for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i) {
        _ear_vk_make_buf(
            size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &buf->ubuf.buffers[i], &buf->ubuf.memories[i]
            );

        vkMapMemory(_ear_vk_device, buf->ubuf.memories[i], 0, size, 0, &buf->ubuf.datas[i]);
    }

    VkDescriptorPoolSize poolsize = {
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = EAR_VK_MAX_FRAMES_IN_FLIGHT,
        };

    VkDescriptorPoolCreateInfo poolinfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .poolSizeCount = 1,
        .pPoolSizes    = &poolsize,

        .maxSets = EAR_VK_MAX_FRAMES_IN_FLIGHT,
        };

    eat_assert(vkCreateDescriptorPool(_ear_vk_device, &poolinfo, NULL, &buf->ubuf.pool) == VK_SUCCESS,
        "failed to create descriptor pool!");

    VkDescriptorSetLayoutBinding layoutbind = {
        .binding = desc.binding,

        .descriptorType  = _ear_vk_convert_desc_type(desc.type),
        .descriptorCount = 1,

        .stageFlags = _ear_vk_convert_stage(desc.stage),

        .pImmutableSamplers = NULL,
        };

    VkDescriptorSetLayoutCreateInfo createinfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .bindingCount = 1,
        .pBindings    = &layoutbind,
        };

    eat_assert(vkCreateDescriptorSetLayout(_ear_vk_device, &createinfo, NULL, &buf->ubuf.layout) == VK_SUCCESS,
        "failed to create descriptor set layout!");

    VkDescriptorSetLayout chips[EAR_VK_MAX_FRAMES_IN_FLIGHT];
    for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i)
        chips[i] = buf->ubuf.layout;

    VkDescriptorSetAllocateInfo allocinfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = NULL,

        .descriptorPool = buf->ubuf.pool,

        .descriptorSetCount = EAR_VK_MAX_FRAMES_IN_FLIGHT,
        .pSetLayouts = chips,
        };

    eat_assert(vkAllocateDescriptorSets(_ear_vk_device, &allocinfo, buf->ubuf.sets) == VK_SUCCESS,
        "failed to create descriptor sets!");
}

void
_ear_vk_update_buf_u(
    ear_vk_buffer* buf,
    void* data,
    uint32_t size
    ) {
    memcpy(buf->ubuf.datas[_ear_vk_cur_img_index], data, size);
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
    case EAR_BUF_INDEX:   return flags | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }

    eat_unreachable();
}
VkDescriptorType
_ear_vk_convert_desc_type(
    ear_buffer_type type
    ) {
    switch (type) {
    case EAR_BUF_VERTEX:  
    case EAR_BUF_INDEX: eat_error("shader pipeline buffer attribs cannot be vertex/index buffers!");

    case EAR_BUF_UNIFORM: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case EAR_BUF_STORAGE: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
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
