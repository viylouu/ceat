#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../pipeline.h"

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
