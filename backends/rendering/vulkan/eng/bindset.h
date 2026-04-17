#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ear/ll/bindset.h"
#include "../vk.h"

typedef struct ear_vk_bindset ear_vk_bindset;

void*
ear_vk_create_bindset(
    ear_bindset_desc desc
    );
void
ear_vk_delete_bindset(
    void* set
    );

void
ear_vk_update_bindset(
    void* set,
    ear_bindset_desc desc
    );

void
ear_vk_bind_bindset(
    void* set,
    uint32_t slot,
    const uint32_t* offsets,
    uint32_t offset_amt
    );

struct ear_vk_bindset{
    ear_bindset_desc desc;

    uint32_t dynamics;

    VkDescriptorSetLayout lay;
    VkDescriptorPool pool;
    VkDescriptorSet sets[EAR_VK_MAX_FRAMES_IN_FLIGHT];
};
