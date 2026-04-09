#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ear/ll/pipeline.h"

typedef struct ear_vk_pipeline ear_vk_pipeline;

extern ear_vk_pipeline* _ear_vk_cur_pipeline;

void*
ear_vk_create_pipeline(
    ear_pipeline_desc desc
    );
void
ear_vk_delete_pipeline(
    void* pln
    );

void
ear_vk_bind_pipeline(
    void* pln
    );

struct ear_vk_pipeline{
    VkPipelineLayout layout;
    VkPipeline pipeline;

    VkDescriptorSetLayout* chips;
        uint32_t chip_amt;
    //struct _ear_vk_pipeline_sets* sets;
    //    uint32_t set_amt;
};
