#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <stdbool.h>

extern bool _ear_vk_in_pass;

void
_ear_vk_start_render_pass(
    uint32_t index,
    uint32_t frame
    );
void
_ear_vk_end_render_pass(
    uint32_t frame
    );
