#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

extern VkPhysicalDevice _ear_vk_physical_device;
extern VkPhysicalDeviceProperties _ear_vk_physical_device_props;

static const char* _ear_vk_device_extensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};
static const uint32_t _ear_vk_device_exension_amt = 1;

void
_ear_vk_pick_physical_device(
    void
    );
