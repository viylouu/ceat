#pragma once

#include <stdint.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../vk.h"

extern VkSemaphore _ear_vk_image_available_sems[EAR_VK_MAX_FRAMES_IN_FLIGHT];
extern VkSemaphore _ear_vk_render_finish_sems[EAR_VK_MAX_FRAMES_IN_FLIGHT];
extern VkFence _ear_vk_inflight_fences[EAR_VK_MAX_FRAMES_IN_FLIGHT];

void
_ear_vk_create_sync_objects(
    void
    );
void
_ear_vk_delete_sync_objects(
    void
    );

void
_ear_vk_wait_for_fences(
    uint32_t frame
    );
