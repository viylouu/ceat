#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct _ear_vk_swapchain_support_details _ear_vk_swapchain_support_details;


_ear_vk_swapchain_support_details
_ear_vk_query_swapchain_support(
    VkPhysicalDevice device
    );


struct _ear_vk_swapchain_support_details{
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* formats;
        uint32_t format_amt;
    VkPresentModeKHR* present_modes;
        uint32_t present_mode_amt;
};
