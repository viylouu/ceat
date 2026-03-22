#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../pipeline.h"

typedef struct _ear_vk_pipeline _ear_vk_pipeline;

_ear_vk_pipeline*
_ear_vk_create_pipeline(
    ear_pipeline_desc desc
    );
void
_ear_vk_delete_pipeline(
    _ear_vk_pipeline* pln
    );

void
_ear_vk_bind_pipeline(
    _ear_vk_pipeline* pln
    );

struct _ear_vk_pipeline{
    VkPipelineLayout layout;
    VkPipeline pipeline;
};
