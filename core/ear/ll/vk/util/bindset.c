#include "bindset.h"
#include "../../../../cutil.h"

#include "buffer.h"
#include "../init/device_log.h"
#include "../../texture.h"
#include "../eng/texture.h"

void
_ear_vk_make_bindset_pool(
    ear_vk_bindset* set,
    ear_bindset_desc desc
    ) {
    uint32_t ubufs = 0; uint32_t sbufs = 0; uint32_t samps = 0;
    for (uint32_t i = 0; i < desc.binding_amt; ++i) {
        switch (desc.bindings[i].type) {
            case EAR_BIND_UNIFORM:   ubufs++; break;
            case EAR_BIND_STORAGE:   sbufs++; break;
            case EAR_BIND_TEXTURE2D: samps++; break;
            default: eat_unreachable();
        }
    }

    bool has_ubufs = ubufs > 0; bool has_sbufs = sbufs > 0; bool has_samps = samps > 0;
    uint32_t size_amt = has_ubufs + has_sbufs + has_samps;
    VkDescriptorPoolSize* sizes = malloc(sizeof(VkDescriptorPoolSize) * size_amt);

    uint32_t assidx = 0;
    if (has_ubufs) sizes[assidx++] = (VkDescriptorPoolSize){
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = ubufs * EAR_VK_MAX_FRAMES_IN_FLIGHT,
        };
    if (has_sbufs) sizes[assidx++] = (VkDescriptorPoolSize){
        .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorCount = sbufs * EAR_VK_MAX_FRAMES_IN_FLIGHT,
        };
    if (has_samps) sizes[assidx++] = (VkDescriptorPoolSize){
        .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = samps * EAR_VK_MAX_FRAMES_IN_FLIGHT,
        };

    VkDescriptorPoolCreateInfo poolinfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .poolSizeCount = size_amt,
        .pPoolSizes    = sizes,

        .maxSets = EAR_VK_MAX_FRAMES_IN_FLIGHT,
        };

    eat_assert(vkCreateDescriptorPool(_ear_vk_device, &poolinfo, NULL, &set->pool) == VK_SUCCESS,
        "failed to create descriptor pool!");

    free(sizes);
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

            .descriptorType  = _ear_vk_convert_desc_type(bind_desc.type),
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
            bool is_buffer = desc.bindings[b].type != EAR_BIND_TEXTURE2D;

            VkDescriptorBufferInfo bufferinfo;
            VkDescriptorImageInfo imageinfo;

            if (is_buffer) {
                ear_buffer*    buf  = desc.bindings[b].object;
                ear_vk_buffer* vbuf = buf->vk;

                bufferinfo = (VkDescriptorBufferInfo){
                    .buffer = vbuf->ubuf.buffers[i],
                    .offset = 0,
                    .range  = vbuf->size,
                    };
            } else {
                ear_texture*    tex  = desc.bindings[b].object;
                ear_vk_texture* vtex = tex->vk;

                imageinfo = (VkDescriptorImageInfo){
                    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    .imageView = vtex->imgview,
                    .sampler   = vtex->samp,
                    };
            }

            VkWriteDescriptorSet descwrite = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = NULL,

                .dstSet          = set->sets[i],
                .dstBinding      = desc.bindings[b].binding,
                .dstArrayElement = 0,

                .descriptorType  = _ear_vk_convert_desc_type(desc.bindings[b].type),
                .descriptorCount = 1,

                .pBufferInfo      = is_buffer?  &bufferinfo : NULL,
                .pImageInfo       = !is_buffer? &imageinfo  : NULL,
                .pTexelBufferView = NULL,
                };

            vkUpdateDescriptorSets(_ear_vk_device, 1, &descwrite, 0, NULL);
        }
}

VkDescriptorType
_ear_vk_convert_desc_type(
    ear_bind_type type
    ) {
    switch (type) {
    case EAR_BIND_UNIFORM:   return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case EAR_BIND_STORAGE:   return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case EAR_BIND_TEXTURE2D: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    }

    eat_unreachable();
}
