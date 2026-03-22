#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef enum _ear_vk_shader_type{
    EAR_VK_SHADER_VERTEX,
    EAR_VK_SHADER_FRAGMENT,
} _ear_vk_shader_type;

VkShaderModule
_ear_vk_create_shader_module(
    const char* code,
    uint32_t amt
    );
void
_ear_vk_delete_shader_module(
    VkShaderModule module
    );

VkPipelineShaderStageCreateInfo
_ear_vk_shader_module_info(
    VkShaderModule module,
    _ear_vk_shader_type type
    );
