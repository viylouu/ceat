#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

extern VkFramebuffer* _ear_vk_swapchain_fbufs;

void
_ear_vk_create_framebuffers(
    void
    );
void
_ear_vk_delete_framebuffers(
    void
    );
