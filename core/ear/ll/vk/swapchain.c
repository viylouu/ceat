#include "swapchain.h"
#include "../../../cutil.h"

#include "surface.h"

_ear_vk_swapchain_support_details
_ear_vk_query_swapchain_support(
    VkPhysicalDevice device
    ) {
    _ear_vk_swapchain_support_details details = {
        .capabilities = 0,
        
        .formats = NULL,
        .format_amt = 0,

        .present_modes = NULL,
        .present_mode_amt = 0,
        };

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _ear_vk_surface, &details.capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(device, _ear_vk_surface, &details.format_amt, NULL);
    details.formats = malloc(sizeof(VkSurfaceFormatKHR) * details.format_amt);
    if (details.format_amt != 0) vkGetPhysicalDeviceSurfaceFormatsKHR(device, _ear_vk_surface, &details.format_amt, details.formats);

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, _ear_vk_surface, &details.present_mode_amt, NULL);
    details.present_modes = malloc(sizeof(VkPresentModeKHR) * details.present_mode_amt);
    if (details.present_mode_amt != 0) vkGetPhysicalDeviceSurfacePresentModesKHR(device, _ear_vk_surface, &details.present_mode_amt, details.present_modes);

    return details;
}
