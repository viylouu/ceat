#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct _ear_vk_swapchain_support_details _ear_vk_swapchain_support_details;

extern VkSwapchainKHR _ear_vk_swapchain;
extern VkImage* _ear_vk_swapchain_imgs;
    extern uint32_t _ear_vk_swapchain_img_amt;
extern VkFormat _ear_vk_swapchain_img_fmt;
extern VkExtent2D _ear_vk_swapchain_extent;


_ear_vk_swapchain_support_details
_ear_vk_query_swapchain_support(
    VkPhysicalDevice device
    );

void
_ear_vk_create_swapchain(
    void
    );
void
_ear_vk_delete_swapchain(
    void
    );

uint32_t
_ear_vk_acquire_swapchain_image(
    uint32_t frame
    );


struct _ear_vk_swapchain_support_details{
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* formats;
        uint32_t format_amt;
    VkPresentModeKHR* present_modes;
        uint32_t present_mode_amt;
};
