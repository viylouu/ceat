#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../pipeline.h"
#include "../eng/pipeline.h"

VkPipelineVertexInputStateCreateInfo
_ear_vk_make_pln_vertexinput(
    ear_pipeline_desc desc,
    VkVertexInputBindingDescription** binds,
    VkVertexInputAttributeDescription** attribs
    );
VkPipelineInputAssemblyStateCreateInfo
_ear_vk_make_pln_inputass(
    ear_pipeline_desc desc
    );
VkPipelineViewportStateCreateInfo
_ear_vk_make_pln_viewport(
    void
    );
VkPipelineRasterizationStateCreateInfo
_ear_vk_make_pln_rasterizer(
    ear_pipeline_desc desc
    );
VkPipelineMultisampleStateCreateInfo
_ear_vk_make_pln_multisample(
    void
    );
VkPipelineColorBlendStateCreateInfo
_ear_vk_make_pln_blend(
    ear_pipeline_desc desc,
    VkPipelineColorBlendAttachmentState* attach
    );

void
_ear_vk_make_pln_layout(
    ear_vk_pipeline* pln,
    ear_pipeline_desc desc
    );

VkFormat
_ear_vk_convert_prim_fmt(
    ear_primitive_type type,
    uint32_t comps,
    bool norm
    );
VkPrimitiveTopology
_ear_vk_convert_topology(
    ear_topology top
    );
VkPolygonMode
_ear_vk_convert_fill(
    ear_fill_mode fill
    );
VkCullModeFlags
_ear_vk_convert_cull(
    ear_cull_mode cull
    );
VkFrontFace
_ear_vk_convert_front(
    ear_front_face front
    );
VkBlendFactor
_ear_vk_convert_blend_fac(
    ear_blend_factor fac
    );
VkBlendOp
_ear_vk_convert_blend_op(
    ear_blend_op op
    );
