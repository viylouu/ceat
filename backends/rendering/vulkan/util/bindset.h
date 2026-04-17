#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ear/ll/bindset.h"
#include "../eng/bindset.h"

void
_ear_vk_make_bindset_pool(
    ear_vk_bindset* set,
    ear_bindset_desc desc
    );
void
_ear_vk_make_bindset_lay(
    ear_vk_bindset* set,
    ear_bindset_desc desc
    );
void
_ear_vk_make_bindset_sets(
    ear_vk_bindset* set
    );

void
_ear_vk_update_bindset_sets(
    ear_vk_bindset* set,
    ear_bindset_desc desc
    );

VkDescriptorType
_ear_vk_convert_desc_type(
    ear_bind_type type
    );
