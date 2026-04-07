#include "pipeline.h"
#include "../../../../cutil.h"

#include "../eng/bindset.h"
#include "../init/device_log.h"
#include "macros.h"

VkPipelineVertexInputStateCreateInfo
_ear_vk_make_pln_vertexinput(
    ear_pipeline_desc desc,
    VkVertexInputBindingDescription** binds,
    VkVertexInputAttributeDescription** attribs
    ) {
    *binds = malloc(sizeof(VkVertexInputBindingDescription) * desc.vertex_buffer_amt);
    for (uint32_t i = 0; i < desc.vertex_buffer_amt; ++i) {
        (*binds)[i] = (VkVertexInputBindingDescription){
            .binding = i,
            .stride = desc.vertex_buffers[i]->desc.stride,
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
            };
    }

    *attribs = malloc(sizeof(VkVertexInputAttributeDescription) * desc.vertex_attrib_amt);
    for (uint32_t i = 0; i < desc.vertex_attrib_amt; ++i) {
        (*attribs)[i] = (VkVertexInputAttributeDescription){
            .binding = desc.vertex_attribs[i].slot,
            .location = desc.vertex_attribs[i].location,
            .offset = desc.vertex_attribs[i].offset,
            .format = _ear_vk_convert_prim_fmt(
                desc.vertex_attribs[i].type, 
                desc.vertex_attribs[i].components, 
                desc.vertex_attribs[i].norm
                ),
            };
    }

    return (VkPipelineVertexInputStateCreateInfo){
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .vertexBindingDescriptionCount = desc.vertex_buffer_amt,
        .pVertexBindingDescriptions    = *binds,

        .vertexAttributeDescriptionCount = desc.vertex_attrib_amt,
        .pVertexAttributeDescriptions    = *attribs,
        };
}
VkPipelineInputAssemblyStateCreateInfo
_ear_vk_make_pln_inputass(
    ear_pipeline_desc desc
    ) {
    return (VkPipelineInputAssemblyStateCreateInfo){
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .topology               = _ear_vk_convert_topology(desc.topology),
        .primitiveRestartEnable = false,
        };
}
VkPipelineViewportStateCreateInfo
_ear_vk_make_pln_viewport(
    void
    ) {
    return (VkPipelineViewportStateCreateInfo){
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .viewportCount = 1,
        .pViewports    = NULL,

        .scissorCount = 1,
        .pScissors    = NULL,
        };
}
VkPipelineRasterizationStateCreateInfo
_ear_vk_make_pln_rasterizer(
    ear_pipeline_desc desc
    ) {
    return (VkPipelineRasterizationStateCreateInfo){
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .depthClampEnable = desc.depth_clamp,

        .rasterizerDiscardEnable = false,

        .polygonMode = _ear_vk_convert_fill(desc.fill_mode),

        .lineWidth = 1,

        .cullMode  = _ear_vk_convert_cull(desc.cull_mode),
        .frontFace = _ear_vk_convert_front(desc.front_face),

        .depthBiasEnable         = false,
        .depthBiasConstantFactor = 0,
        .depthBiasClamp          = 0,
        .depthBiasSlopeFactor    = 0,
        };
}
VkPipelineMultisampleStateCreateInfo
_ear_vk_make_pln_multisample(
    void
    ) {
    return (VkPipelineMultisampleStateCreateInfo){
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .sampleShadingEnable   = false,
        .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
        .minSampleShading      = 1,
        .pSampleMask           = NULL,
        .alphaToCoverageEnable = false,
        .alphaToOneEnable      = false,
        };
}
VkPipelineColorBlendStateCreateInfo
_ear_vk_make_pln_blend(
    ear_pipeline_desc desc,
    VkPipelineColorBlendAttachmentState* attach
    ) {
    *attach = (VkPipelineColorBlendAttachmentState){
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                          VK_COLOR_COMPONENT_G_BIT |
                          VK_COLOR_COMPONENT_B_BIT |
                          VK_COLOR_COMPONENT_A_BIT,

        .blendEnable = desc.has_blend_state,

        .srcColorBlendFactor = _ear_vk_convert_blend_fac(desc.blend_state.src_color),
        .dstColorBlendFactor = _ear_vk_convert_blend_fac(desc.blend_state.dst_color),
        .colorBlendOp        = _ear_vk_convert_blend_op(desc.blend_state.color_op),

        .srcAlphaBlendFactor = _ear_vk_convert_blend_fac(desc.blend_state.src_alpha),
        .dstAlphaBlendFactor = _ear_vk_convert_blend_fac(desc.blend_state.dst_alpha),
        .alphaBlendOp        = _ear_vk_convert_blend_op(desc.blend_state.alpha_op),
        };

    return (VkPipelineColorBlendStateCreateInfo){
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .logicOpEnable = false,
        .logicOp       = VK_LOGIC_OP_COPY,

        .attachmentCount = 1,
        .pAttachments    = attach,

        .blendConstants[0] = 0,
        .blendConstants[1] = 0,
        .blendConstants[2] = 0,
        .blendConstants[3] = 0,
        };
}

void
_ear_vk_make_pln_layout(
    ear_vk_pipeline* pln,
    ear_pipeline_desc desc
    ) {
    pln->chips = malloc(sizeof(VkDescriptorSetLayout) * desc.bindset_amt);
    pln->chip_amt = desc.bindset_amt;
    for (uint32_t i = 0; i < desc.bindset_amt; ++i)
        pln->chips[i] = get_vk(bindset, desc.bindsets[i])->lay;

    VkPipelineLayoutCreateInfo layoutinfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .setLayoutCount = pln->chip_amt,
        .pSetLayouts    = pln->chips,

        .pushConstantRangeCount = 0,
        .pPushConstantRanges    = NULL,
        };

    eat_assert(vkCreatePipelineLayout(_ear_vk_device, &layoutinfo, NULL, &pln->layout) == VK_SUCCESS, 
        "failed to create shader pipeline layout!");
}

VkFormat
_ear_vk_convert_prim_fmt(
    ear_primitive_type type,
    uint32_t comps,
    bool norm
    ) {
    switch (type) {
    case EAR_PRIM_FLOAT:
        eat_assert(!norm, "floats cannot be normalized!");

        switch (comps) {
        case 1: return VK_FORMAT_R32_SFLOAT;
        case 2: return VK_FORMAT_R32G32_SFLOAT;
        case 3: return VK_FORMAT_R32G32B32_SFLOAT;
        case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        eat_unreachable();
    case EAR_PRIM_INT:
        eat_assert(!norm, "32 bit integers cannot be normalized!");

        switch (comps) {
        case 1: return VK_FORMAT_R32_SINT;
        case 2: return VK_FORMAT_R32G32_SINT;
        case 3: return VK_FORMAT_R32G32B32_SINT;
        case 4: return VK_FORMAT_R32G32B32A32_SINT;
        }
        eat_unreachable();
    }

    eat_unreachable();
}
VkPrimitiveTopology
_ear_vk_convert_topology(
    ear_topology top
    ) {
    switch (top) {
    case EAR_TOP_TRIS:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case EAR_TOP_LINES: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    }

    eat_unreachable();
}
VkPolygonMode
_ear_vk_convert_fill(
    ear_fill_mode fill
    ) {
    switch (fill) {
    case EAR_FILL_FILL: return VK_POLYGON_MODE_FILL;
    case EAR_FILL_LINE: return VK_POLYGON_MODE_LINE;
    }

    eat_unreachable();
}
VkCullModeFlags
_ear_vk_convert_cull(
    ear_cull_mode cull
    ) {
    switch (cull) {
    case EAR_CULL_NONE:  return VK_CULL_MODE_NONE;
    case EAR_CULL_BACK:  return VK_CULL_MODE_BACK_BIT;
    case EAR_CULL_FRONT: return VK_CULL_MODE_FRONT_BIT;
    }

    eat_unreachable();
}
VkFrontFace
_ear_vk_convert_front(
    ear_front_face front
    ) {
    switch (front) {
    case EAR_FRONT_CW:  return VK_FRONT_FACE_CLOCKWISE;
    case EAR_FRONT_CCW: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }

    eat_unreachable();
}
VkBlendFactor
_ear_vk_convert_blend_fac(
    ear_blend_factor fac
    ) {
    switch (fac) {
    case EAR_FAC_ONE:           return VK_BLEND_FACTOR_ONE;
    case EAR_FAC_ZERO:          return VK_BLEND_FACTOR_ZERO;
    case EAR_FAC_SRC_COLOR:     return VK_BLEND_FACTOR_SRC_COLOR;
    case EAR_FAC_DST_COLOR:     return VK_BLEND_FACTOR_DST_COLOR;
    case EAR_FAC_INV_SRC_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case EAR_FAC_INV_DST_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case EAR_FAC_SRC_ALPHA:     return VK_BLEND_FACTOR_SRC_ALPHA;
    case EAR_FAC_DST_ALPHA:     return VK_BLEND_FACTOR_DST_ALPHA;
    case EAR_FAC_INV_SRC_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case EAR_FAC_INV_DST_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    }

    eat_unreachable();
}
VkBlendOp
_ear_vk_convert_blend_op(
    ear_blend_op op
    ) {
    switch (op) {
    case EAR_OP_ADD:          return VK_BLEND_OP_ADD;
    case EAR_OP_SUBTRACT:     return VK_BLEND_OP_SUBTRACT;
    case EAR_OP_REV_SUBTRACT: return VK_BLEND_OP_REVERSE_SUBTRACT;
    case EAR_OP_MIN:          return VK_BLEND_OP_MIN;
    case EAR_OP_MAX:          return VK_BLEND_OP_MAX;
    }

    eat_unreachable();
}
