#include "validation.h"
#include "cutil.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string.h>

bool
_ear_vk_validation_support(
    void
    ) {
    uint32_t layeramt;
    vkEnumerateInstanceLayerProperties(&layeramt, NULL);

    VkLayerProperties* available = malloc(sizeof(VkLayerProperties) * layeramt);
    vkEnumerateInstanceLayerProperties(&layeramt, available);

    for (uint32_t i = 0; i < _ear_vk_validation_amt; ++i) {
        for (uint32_t j = 0; j < layeramt; ++j) if (!strcmp(_ear_vk_validations[i], available[j].layerName))
            goto next;
        eat_warn("unsupported validation layer \"%s\"!", _ear_vk_validations[i]);
        return false;
next:
        continue;
    }

    return true;
}
