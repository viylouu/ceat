#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

extern VkRenderPass _ear_vk_renderpass;

void
_ear_vk_create_render_pass(
    void
    );
void
_ear_vk_delete_render_pass(
    void
    );
