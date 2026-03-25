#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../pipeline.h"

typedef struct ear_vk_pipeline ear_vk_pipeline;

extern ear_vk_pipeline* _ear_vk_cur_pipeline;

ear_vk_pipeline*
ear_vk_create_pipeline(
    ear_pipeline_desc desc
    );
void
ear_vk_delete_pipeline(
    ear_vk_pipeline* pln
    );

void
ear_vk_bind_pipeline(
    ear_vk_pipeline* pln
    );

struct ear_vk_pipeline{
    VkPipelineLayout layout;
    VkPipeline pipeline;

    VkDescriptorSetLayout* setlayouts;
        uint32_t setlayout_amt;
};
