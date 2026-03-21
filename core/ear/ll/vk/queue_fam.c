#include "queue_fam.h"
#include "../../../cutil.h"

#include "surface.h"

_ear_vk_queue_family_inds
_ear_vk_find_queue_families(
    VkPhysicalDevice device
    ) {
    _ear_vk_queue_family_inds inds = {
        .graphics_family = -1,
        .present_family = -1,
        .complete = false,
        };

    uint32_t queuefamamt = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queuefamamt, NULL);

    VkQueueFamilyProperties* queuefams = malloc(sizeof(VkQueueFamilyProperties) * queuefamamt);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queuefamamt, queuefams);

    for (uint32_t i = 0; i < queuefamamt; ++i) {
        if (queuefams[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) inds.graphics_family = i;
        
        VkBool32 canpresent = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _ear_vk_surface, &canpresent);
        if (canpresent) inds.present_family = i;
    }

    inds.complete = inds.graphics_family != -1 && inds.present_family != -1;
    if (!inds.complete) eat_warn("queue families incomplete!");

    return inds;
}
