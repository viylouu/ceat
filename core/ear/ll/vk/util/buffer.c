#include "buffer.h"
#include "../../../../cutil.h"

#include "pipeline.h"

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
_ear_vk_make_buf_stage(
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
_ear_vk_make_buf_pers(
    ear_vk_buffer* buf,
    ear_buffer_desc desc,
    void* data,
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
    }
}

void
_ear_vk_del_buf_stage(
    ear_vk_buffer* buf
    ) {
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

    if (buf->ubuf.has_sets)
        vkDestroyDescriptorPool(_ear_vk_device, buf->ubuf.pool, NULL);
}

void
_ear_vk_make_buf_set(
    ear_vk_buffer* buf,
    ear_buffer_bind_set set
    ) {
    uint32_t sizes = 0;
    ear_buffer_type size_types[2];
    uint32_t sbuf_amt = 0; uint32_t ubuf_amt = 0;

    for (uint32_t i = 0; i < set.binding_amt; ++i) {
        uint32_t pubuf = ubuf_amt;
        uint32_t psbuf = sbuf_amt;

        switch(set.bindings[i].buffer->desc.type) {
        case EAR_BUF_UNIFORM: ++ubuf_amt; break;
        case EAR_BUF_STORAGE_PERSISTENT:
        case EAR_BUF_STORAGE_STAGING: ++sbuf_amt; break;
        default: eat_unreachable();
        }

        if (pubuf != ubuf_amt && pubuf == 0 ||
            psbuf != sbuf_amt && psbuf == 0) {
            size_types[sizes] = pubuf == 0? EAR_BUF_UNIFORM :
                                psbuf == 0? EAR_BUF_STORAGE_PERSISTENT : // goon
                                0;
            ++sizes;
        }
    }

    VkDescriptorPoolSize* poolsizes = malloc(sizeof(VkDescriptorPoolSize) * sizes); 
    for (uint32_t i = 0; i < sizes; ++i) {
        poolsizes[i] = (VkDescriptorPoolSize){
            .type = _ear_vk_convert_desc_type(size_types[i]),
            .descriptorCount = EAR_VK_MAX_FRAMES_IN_FLIGHT * (
                                size_types[i] == EAR_BUF_UNIFORM? ubuf_amt :
                                size_types[i] == EAR_BUF_STORAGE_PERSISTENT? sbuf_amt :
                                0
                                ),
            };
    }

    VkDescriptorPoolCreateInfo poolinfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .poolSizeCount = sizes,
        .pPoolSizes    = poolsizes,

        .maxSets = EAR_VK_MAX_FRAMES_IN_FLIGHT,
        };

    eat_assert(vkCreateDescriptorPool(_ear_vk_device, &poolinfo, NULL, &buf->ubuf.pool) == VK_SUCCESS,
        "failed to create descriptor pool!");

    VkDescriptorSetLayout lay = _ear_vk_convert_bind_set(set);

    VkDescriptorSetLayout chips[EAR_VK_MAX_FRAMES_IN_FLIGHT];
    for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i)
        chips[i] = lay;

    VkDescriptorSetAllocateInfo allocinfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = NULL,

        .descriptorPool = buf->ubuf.pool,

        .descriptorSetCount = EAR_VK_MAX_FRAMES_IN_FLIGHT,
        .pSetLayouts = chips,
        };

    eat_assert(vkAllocateDescriptorSets(_ear_vk_device, &allocinfo, buf->ubuf.sets) == VK_SUCCESS,
        "failed to create descriptor sets!");

    for (uint32_t b = 0; b < set.binding_amt; ++b)
        for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i) {
            ear_vk_buffer* bbuf = set.bindings[b].buffer->vk;

            VkDescriptorBufferInfo bufferinfo = {
                .buffer = bbuf->ubuf.buffers[i],
                .offset = 0,
                .range  = bbuf->stride,
                };

            VkWriteDescriptorSet descwrite = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = NULL,

                .dstSet          = buf->ubuf.sets[i],
                .dstBinding      = set.bindings[b].binding,
                .dstArrayElement = 0,

                .descriptorType  = _ear_vk_convert_desc_type(bbuf->type),
                .descriptorCount = 1,

                .pBufferInfo      = &bufferinfo,
                .pImageInfo       = NULL,
                .pTexelBufferView = NULL,
                };

            vkUpdateDescriptorSets(_ear_vk_device, 1, &descwrite, 0, NULL);
        }

    vkDestroyDescriptorSetLayout(_ear_vk_device, lay, NULL);
    free(poolsizes);
}

void
_ear_vk_update_buf_pers(
    ear_vk_buffer* buf,
    void* data,
    uint32_t size
    ) {
    memcpy(buf->ubuf.datas[_ear_vk_cur_frame], data, size);
}

VkBufferUsageFlags
_ear_vk_convert_buf_type(
    ear_buffer_type type
    ) {
    VkBufferUsageFlags flags = VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    switch (type) {
    case EAR_BUF_VERTEX:             return flags | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    case EAR_BUF_UNIFORM:            return flags | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    case EAR_BUF_STORAGE_STAGING:
    case EAR_BUF_STORAGE_PERSISTENT: return flags | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    case EAR_BUF_INDEX:              return flags | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
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
    case EAR_BUF_UNIFORM:            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case EAR_BUF_STORAGE_STAGING: 
    case EAR_BUF_STORAGE_PERSISTENT: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    }

    eat_unreachable();
}
VkDescriptorSetLayout
_ear_vk_convert_bind_set(
    ear_buffer_bind_set set
    ) {
    VkDescriptorSetLayoutBinding* bindings = malloc(sizeof(VkDescriptorSetLayoutBinding) * set.binding_amt);
    for (uint32_t i = 0; i < set.binding_amt; ++i) {
        ear_buffer_bind_desc bind_desc = set.bindings[i];
        bindings[i] = (VkDescriptorSetLayoutBinding){
            .binding = bind_desc.binding,

            .descriptorType  = _ear_vk_convert_desc_type(bind_desc.buffer->desc.type),
            .descriptorCount = 1,

            .stageFlags = _ear_vk_convert_stage(bind_desc.stage),

            .pImmutableSamplers = NULL,
            };
    }

    VkDescriptorSetLayoutCreateInfo createinfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .bindingCount = set.binding_amt,
        .pBindings    = bindings,
        };

    VkDescriptorSetLayout out;
    eat_assert(vkCreateDescriptorSetLayout(_ear_vk_device, &createinfo, NULL, &out) == VK_SUCCESS,
        "failed to create descriptor set layout!");

    free(bindings);

    return out;
}
