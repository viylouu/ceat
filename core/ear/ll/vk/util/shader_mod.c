#include "shader_mod.h"
#include "../../../../cutil.h"

#include "../init/device_log.h"

VkShaderModule
_ear_vk_create_shader_module(
    const uint32_t* code,
    uint32_t amt
    ) {
    if (amt % 4 != 0) eat_warn("spirv file not 4 byte aligned! possible errors!");

    VkShaderModuleCreateInfo createinfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .codeSize = amt,
        .pCode    = code,
        };

    VkShaderModule mod;
    eat_assert(vkCreateShaderModule(_ear_vk_device, &createinfo, NULL, &mod) == VK_SUCCESS, 
        "failed to create shader module!");

    return mod;
}
void
_ear_vk_delete_shader_module(
    VkShaderModule module
    ) {
    vkDestroyShaderModule(_ear_vk_device, module, NULL);
}

VkPipelineShaderStageCreateInfo
_ear_vk_shader_module_info(
    VkShaderModule module,
    _ear_vk_shader_type type
    ) {
    VkShaderStageFlagBits stage;
    switch (type) {
    case EAR_VK_SHADER_VERTEX:   stage = VK_SHADER_STAGE_VERTEX_BIT;   break;
    case EAR_VK_SHADER_FRAGMENT: stage = VK_SHADER_STAGE_FRAGMENT_BIT; break;
    }

    return (VkPipelineShaderStageCreateInfo){
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .stage = stage,

        .module = module,
        .pName  = "main",

        .pSpecializationInfo = NULL,
        };
}
