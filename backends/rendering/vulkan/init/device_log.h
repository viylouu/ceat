#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

extern VkDevice _ear_vk_device;
extern VkQueue _ear_vk_graphics_queue;
extern VkQueue _ear_vk_present_queue;

void
_ear_vk_create_logical_device(
    void
    );
void
_ear_vk_delete_logical_device(
    void
    );

void
_ear_vk_device_wait_idle(
    void
    );
