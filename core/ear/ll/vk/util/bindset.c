#include "bindset.h"
#include "../../../../cutil.h"

#include "buffer.h"
#include "../init/device_log.h"

void
_ear_vk_make_bindset_pool(
    ear_vk_bindset* set,
    ear_bindset_desc desc
    ) {
    uint32_t ubufs = 0; uint32_t sbufs = 0;
    for (uint32_t i = 0; i < desc.binding_amt; ++i) {
        switch (desc.bindings[i].buffer->desc.type) {
            case EAR_BUF_UNIFORM:
                ubufs++; break;
            case EAR_BUF_STORAGE_PERSISTENT:
            case EAR_BUF_STORAGE_STAGING:
                sbufs++; break;
            default: eat_unreachable();
        }
    }

    VkDescriptorPoolSize sizes[2] = {
        (VkDescriptorPoolSize){
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = ubufs * EAR_VK_MAX_FRAMES_IN_FLIGHT,
            }, 
        (VkDescriptorPoolSize){
            .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .descriptorCount = sbufs * EAR_VK_MAX_FRAMES_IN_FLIGHT,
            },
        };

    VkDescriptorPoolCreateInfo poolinfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .poolSizeCount = 2,
        .pPoolSizes    = sizes,

        .maxSets = EAR_VK_MAX_FRAMES_IN_FLIGHT,
        };

    eat_assert(vkCreateDescriptorPool(_ear_vk_device, &poolinfo, NULL, &set->pool) == VK_SUCCESS,
        "failed to create descriptor pool!");
}
void
_ear_vk_make_bindset_lay(
    ear_vk_bindset* set,
    ear_bindset_desc desc
    ) {
    VkDescriptorSetLayoutBinding* bindings = malloc(sizeof(VkDescriptorSetLayoutBinding) * desc.binding_amt);
    for (uint32_t i = 0; i < desc.binding_amt; ++i) {
        ear_bind_desc bind_desc = desc.bindings[i];
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

        .bindingCount = desc.binding_amt,
        .pBindings    = bindings,
        };

    eat_assert(vkCreateDescriptorSetLayout(_ear_vk_device, &createinfo, NULL, &set->lay) == VK_SUCCESS,
        "failed to create descriptor set layout!");

    free(bindings);
}
void
_ear_vk_make_bindset_sets(
    ear_vk_bindset* set,
    ear_bindset_desc desc
    ) {
    VkDescriptorSetLayout chips[EAR_VK_MAX_FRAMES_IN_FLIGHT];
    for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i)
        chips[i] = set->lay;

    VkDescriptorSetAllocateInfo allocinfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = NULL,

        .descriptorPool = set->pool,

        .descriptorSetCount = EAR_VK_MAX_FRAMES_IN_FLIGHT,
        .pSetLayouts = chips,
        };

    eat_assert(vkAllocateDescriptorSets(_ear_vk_device, &allocinfo, set->sets) == VK_SUCCESS,
        "failed to create descriptor sets!");

    for (uint32_t b = 0; b < desc.binding_amt; ++b)
        for (uint32_t i = 0; i < EAR_VK_MAX_FRAMES_IN_FLIGHT; ++i) {
            ear_vk_buffer* bbuf = desc.bindings[b].buffer->vk;

            VkDescriptorBufferInfo bufferinfo = {
                .buffer = bbuf->ubuf.buffers[i],
                .offset = 0,
                .range  = bbuf->size,
                };

            VkWriteDescriptorSet descwrite = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = NULL,

                .dstSet          = set->sets[i],
                .dstBinding      = desc.bindings[b].binding,
                .dstArrayElement = 0,

                .descriptorType  = _ear_vk_convert_desc_type(bbuf->type),
                .descriptorCount = 1,

                .pBufferInfo      = &bufferinfo,
                .pImageInfo       = NULL,
                .pTexelBufferView = NULL,
                };

            vkUpdateDescriptorSets(_ear_vk_device, 1, &descwrite, 0, NULL);
        }
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
