#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../bindset.h"
#include "../vk.h"

typedef struct ear_vk_bindset ear_vk_bindset;

ear_vk_bindset*
ear_vk_create_bindset(
    ear_bindset_desc desc
    );
void
ear_vk_delete_bindset(
    ear_vk_bindset* set
    );

void
ear_vk_bind_bindset(
    ear_vk_bindset* set
    );

struct ear_vk_bindset{
    VkDescriptorSetLayout lay;
    VkDescriptorPool pool;
    VkDescriptorSet sets[EAR_VK_MAX_FRAMES_IN_FLIGHT];
};
