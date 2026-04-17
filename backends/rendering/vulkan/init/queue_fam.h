#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <stdbool.h>

typedef struct _ear_vk_queue_family_inds _ear_vk_queue_family_inds;

_ear_vk_queue_family_inds
_ear_vk_find_queue_families(
    VkPhysicalDevice device
    );

struct _ear_vk_queue_family_inds{
    int32_t graphics_family;
    int32_t present_family;

    bool complete;
};
