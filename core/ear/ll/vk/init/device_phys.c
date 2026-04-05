#include "device_phys.h"
#include "../../../../cutil.h"

#include "queue_fam.h"
#include "instance.h"
#include "../sc/swapchain.h"

#include <string.h>

VkPhysicalDevice _ear_vk_physical_device;
VkPhysicalDeviceProperties _ear_vk_physical_device_props;

bool
_ear_vk_check_device_extension_support(
    VkPhysicalDevice device
    ) {
    uint32_t extensionamt;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionamt, NULL);

    VkExtensionProperties* properties = malloc(sizeof(VkExtensionProperties) * extensionamt);
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionamt, properties);
    
    for (uint32_t i = 0; i < _ear_vk_device_exension_amt; ++i) {
        for (uint32_t j = 0; j < extensionamt; ++j) if (!strcmp(_ear_vk_device_extensions[i], properties[j].extensionName))
            goto next;
        eat_warn("unsupported device extension \"%s\"!", _ear_vk_device_extensions[i]);
        free(properties);
        return false;
next:
    }

    free(properties);
    return true;
}

bool
_ear_vk_is_device_suitable(
    VkPhysicalDevice device
    ) {
    _ear_vk_queue_family_inds inds = _ear_vk_find_queue_families(device);

    bool extensionssupp = _ear_vk_check_device_extension_support(device);

    bool goodswapchain = false;
    if (extensionssupp) {
        _ear_vk_swapchain_support_details details = _ear_vk_query_swapchain_support(device);
        goodswapchain = details.format_amt != 1 && details.present_mode_amt != 0;
        free(details.formats);
        free(details.present_modes);
    }

    VkPhysicalDeviceFeatures feats;
    vkGetPhysicalDeviceFeatures(device, &feats);

    return inds.complete && extensionssupp && goodswapchain &&
        feats.samplerAnisotropy && feats.vertexPipelineStoresAndAtomics;
}

void
_ear_vk_pick_physical_device(
    void
    ) {
    uint32_t deviceamt;
    vkEnumeratePhysicalDevices(_ear_vk_instance, &deviceamt, NULL);

    eat_assert(deviceamt != 0, "failed to find a vulkan supported device!");

    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceamt);
    vkEnumeratePhysicalDevices(_ear_vk_instance, &deviceamt, devices);

    for (uint32_t i = 0; i < deviceamt; ++i) {
        VkPhysicalDevice device = devices[i];
        if (!_ear_vk_is_device_suitable(device)) continue;

        _ear_vk_physical_device = device;
        break;
    }

    vkGetPhysicalDeviceProperties(_ear_vk_physical_device, &_ear_vk_physical_device_props);

    eat_assert(_ear_vk_physical_device != NULL, "failed to find any suitable vulkan supported devices!");
}
