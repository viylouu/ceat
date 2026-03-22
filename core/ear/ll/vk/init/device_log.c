#include "device_log.h"
#include "../../../../cutil.h"

#include "validation.h"
#include "device_phys.h"
#include "queue_fam.h"

VkDevice _ear_vk_device;
VkQueue _ear_vk_graphics_queue;
VkQueue _ear_vk_present_queue;

void
_ear_vk_create_logical_device(
    void
    ) {
    _ear_vk_queue_family_inds inds = _ear_vk_find_queue_families(_ear_vk_physical_device);

    float queuepriority = 1;

    VkDeviceQueueCreateInfo queuecreateinfos[2];
    uint32_t uniques = 2;
    uint32_t families[2] = { inds.graphics_family, inds.present_family };
    if (inds.present_family == inds.graphics_family) uniques = 1;

    for (uint32_t i = 0; i < uniques; ++i) {
        queuecreateinfos[i] = (VkDeviceQueueCreateInfo){
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .queueFamilyIndex = families[i],
            .queueCount = 1,

            .pQueuePriorities = &queuepriority,
            };
    }

    VkPhysicalDeviceFeatures devicefeatures = {0};

    VkDeviceCreateInfo createinfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .queueCreateInfoCount = uniques,
        .pQueueCreateInfos    = queuecreateinfos,

        .enabledLayerCount   = _ear_vk_validation? _ear_vk_validation_amt : 0,
        .ppEnabledLayerNames = _ear_vk_validation? _ear_vk_validations    : NULL,

        .enabledExtensionCount   = _ear_vk_device_exension_amt,
        .ppEnabledExtensionNames = _ear_vk_device_extensions,

        .pEnabledFeatures = &devicefeatures,
        };

    eat_assert(vkCreateDevice(_ear_vk_physical_device, &createinfo, NULL, &_ear_vk_device) == VK_SUCCESS, "failed to create logical device!");

    vkGetDeviceQueue(_ear_vk_device, inds.graphics_family, 0, &_ear_vk_graphics_queue);
    vkGetDeviceQueue(_ear_vk_device, inds.present_family,  0, &_ear_vk_present_queue);
}
void
_ear_vk_delete_logical_device(
    void
    ) {
    vkDestroyDevice(_ear_vk_device, NULL);
}

void
_ear_vk_device_wait_idle(
    void
    ) {
    vkDeviceWaitIdle(_ear_vk_device);
}
