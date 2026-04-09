#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

void
_ear_vk_start_render_pass(
    uint32_t index,
    uint32_t frame
    );
void
_ear_vk_end_render_pass(
    uint32_t frame
    );
