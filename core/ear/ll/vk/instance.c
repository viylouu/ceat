#include "instance.h"
#include "../../../cutil.h"

#include "../../../eaw/vk.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "validation.h"

#include <string.h>

VkInstance _ear_vk_instance;

void 
_ear_vk_create_instance(
    void
    ) {
    VkApplicationInfo appinfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,

        .pApplicationName = "appname",
        .applicationVersion = VK_MAKE_VERSION(1,0,0),

        .pEngineName = "eat",
        .engineVersion = VK_MAKE_VERSION(1,0,0),

        .apiVersion = VK_API_VERSION_1_0,
        };

    uint32_t extensionamt;
    const char** extensions;
    extensions = _eaw_vk_get_instance_extensions(&extensionamt);

    uint32_t actualexts;
    vkEnumerateInstanceExtensionProperties(NULL, &actualexts, NULL);

    VkExtensionProperties* properties = malloc(sizeof(VkExtensionProperties) * actualexts);
    vkEnumerateInstanceExtensionProperties(NULL, &actualexts, properties);

    for (uint32_t i = 0; i < extensionamt; ++i) {
        for (uint32_t j = 0; j < actualexts; ++j) if (!strcmp(extensions[i], properties[j].extensionName))
            goto next;
        eat_error("unsupported extension \"%s\"!", extensions[i]);
next:
    }

    free(properties);

    VkInstanceCreateInfo createinfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .pApplicationInfo = &appinfo,

        .enabledLayerCount   = _ear_vk_validation? _ear_vk_validation_amt : 0,
        .ppEnabledLayerNames = _ear_vk_validation? _ear_vk_validations    : NULL,

        .enabledExtensionCount   = extensionamt,
        .ppEnabledExtensionNames = extensions,
        };

    eat_assert(vkCreateInstance(&createinfo, NULL, &_ear_vk_instance) == VK_SUCCESS, "failed to create vulkan instance!");
}
void
_ear_vk_delete_instance(
    void
    ) {
    vkDestroyInstance(_ear_vk_instance, NULL);
}
