#include "pipeline.h"
#include "../../../../cutil.h"

#include "../../pipeline.h"

#include "../sc/render_pass.h"
#include "core/ear/ll/buffer.h"
#include "shader_mod.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "../init/device_log.h"

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

VkDescriptorType
_ear_vk_convert_desc_type(
    ear_buffer_type type
    ) {
    switch (type) {
    case EAR_BUF_VERTEX:  eat_error("shader pipeline buffer attribs cannot be vertex buffers!");
    case EAR_BUF_UNIFORM: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case EAR_BUF_STORAGE: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    }

    eat_unreachable();
}

ear_vk_pipeline*
ear_vk_create_pipeline(
    ear_pipeline_desc desc
    ) {
    ear_vk_pipeline* pln = malloc(sizeof(ear_vk_pipeline));

    VkShaderModule vertex   = _ear_vk_create_shader_module(desc.vertex.source,   desc.vertex.source_size);
    VkShaderModule fragment = _ear_vk_create_shader_module(desc.fragment.source, desc.fragment.source_size);

    VkPipelineShaderStageCreateInfo stages[] = {
        _ear_vk_shader_module_info(vertex,   EAR_VK_SHADER_VERTEX),
        _ear_vk_shader_module_info(fragment, EAR_VK_SHADER_FRAGMENT),
        };

    VkDynamicState dstates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        };
    VkPipelineDynamicStateCreateInfo dstate = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .dynamicStateCount = sizeof(dstates) / sizeof(dstates[0]),
        .pDynamicStates    = dstates,
        };

    VkVertexInputBindingDescription* bindingdescs = malloc(sizeof(VkVertexInputBindingDescription) * desc.vertex_buffer_amt);
    for (uint32_t i = 0; i < desc.vertex_buffer_amt; ++i) {
        bindingdescs[i] = (VkVertexInputBindingDescription){
            .binding = i,
            .stride = desc.vertex_buffers[i]->desc.stride,
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
            };
    }

    VkVertexInputAttributeDescription* attribdescs = malloc(sizeof(VkVertexInputAttributeDescription) * desc.vertex_attrib_amt);
    for (uint32_t i = 0; i < desc.vertex_attrib_amt; ++i) {
        attribdescs[i] = (VkVertexInputAttributeDescription){
            .binding = desc.vertex_attribs[i].slot,
            .location = desc.vertex_attribs[i].location,
            .offset = desc.vertex_attribs[i].offset,
            .format = _ear_vk_convert_prim_fmt(desc.vertex_attribs[i].type, desc.vertex_attribs[i].components, desc.vertex_attribs[i].norm),
            };
    }

    VkPipelineVertexInputStateCreateInfo vertexinputinfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .vertexBindingDescriptionCount = desc.vertex_buffer_amt,
        .pVertexBindingDescriptions    = bindingdescs,

        .vertexAttributeDescriptionCount = desc.vertex_attrib_amt,
        .pVertexAttributeDescriptions    = attribdescs,
        };

    VkPipelineInputAssemblyStateCreateInfo inputassembly = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .topology               = _ear_vk_convert_topology(desc.topology),
        .primitiveRestartEnable = false,
        };

    VkPipelineViewportStateCreateInfo viewportstate = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .viewportCount = 1,
        .pViewports    = NULL,

        .scissorCount = 1,
        .pScissors    = NULL,
        };

    VkPipelineRasterizationStateCreateInfo rasterizer = {
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

    VkPipelineMultisampleStateCreateInfo multisampling = {
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

    VkPipelineColorBlendAttachmentState colorblendattach = {
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
    VkPipelineColorBlendStateCreateInfo colorblend = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .logicOpEnable = false,
        .logicOp       = VK_LOGIC_OP_COPY,

        .attachmentCount = 1,
        .pAttachments    = &colorblendattach,

        .blendConstants[0] = 0,
        .blendConstants[1] = 0,
        .blendConstants[2] = 0,
        .blendConstants[3] = 0,
        };

    pln->setlayouts = malloc(sizeof(VkDescriptorSetLayout) * desc.buffer_attrib_set_amt);
    pln->setlayout_amt = desc.buffer_attrib_set_amt;

    for (uint32_t i = 0; i < desc.buffer_attrib_set_amt; ++i) {
        ear_buffer_attrib_desc_set set = desc.buffer_attrib_sets[i];
        VkDescriptorSetLayoutBinding* bindings = malloc(sizeof(VkDescriptorSetLayoutBinding) * set.buffer_attrib_amt);
        for (uint32_t j = 0; j < set.buffer_attrib_amt; ++j) {
            ear_buffer_attrib_desc attrib = set.buffer_attribs[j];
            bindings[j] = (VkDescriptorSetLayoutBinding){
                .binding = attrib.binding,

                .descriptorType = _ear_vk_convert_desc_type(attrib.type),
                .descriptorCount = 1,

                .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,

                .pImmutableSamplers = NULL,
                };
        }

        VkDescriptorSetLayoutCreateInfo createinfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = NULL,

            .flags = 0,

            .bindingCount = set.buffer_attrib_amt,
            .pBindings    = bindings,
            };

        eat_assert(vkCreateDescriptorSetLayout(_ear_vk_device, &createinfo, NULL, &pln->setlayouts[i]) == VK_SUCCESS,
            "failed to create descriptor set layout!");

        free(bindings);
    }

    VkPipelineLayoutCreateInfo layoutinfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .setLayoutCount = pln->setlayout_amt,
        .pSetLayouts    = pln->setlayouts,

        .pushConstantRangeCount = 0,
        .pPushConstantRanges    = NULL,
        };

    eat_assert(vkCreatePipelineLayout(_ear_vk_device, &layoutinfo, NULL, &pln->layout) == VK_SUCCESS, 
        "failed to create shader pipeline layout!");

    VkGraphicsPipelineCreateInfo createinfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .stageCount = 2,
        .pStages    = stages,

        .pVertexInputState   = &vertexinputinfo,
        .pInputAssemblyState = &inputassembly,
        .pTessellationState  = NULL,
        .pViewportState      = &viewportstate,
        .pRasterizationState = &rasterizer,
        .pMultisampleState   = &multisampling,
        .pDepthStencilState  = NULL,
        .pColorBlendState    = &colorblend,
        .pDynamicState       = &dstate,

        .layout = pln->layout,

        .renderPass = _ear_vk_renderpass,
        .subpass    = 0,

        .basePipelineHandle = NULL,
        .basePipelineIndex  = -1,
        };

    eat_assert(vkCreateGraphicsPipelines(_ear_vk_device, NULL, 1, &createinfo, NULL, &pln->pipeline) == VK_SUCCESS, 
        "failed to create graphics pipeline!");

    _ear_vk_delete_shader_module(vertex);
    _ear_vk_delete_shader_module(fragment);

    free(bindingdescs);
    free(attribdescs);

    return pln;
}
void
ear_vk_delete_pipeline(
    ear_vk_pipeline* pln
    ) {
    _ear_vk_device_wait_idle();

    vkDestroyPipeline(_ear_vk_device, pln->pipeline, NULL);
    vkDestroyPipelineLayout(_ear_vk_device, pln->layout, NULL);

    for (uint32_t i = 0; i < pln->setlayout_amt; ++i)
        vkDestroyDescriptorSetLayout(_ear_vk_device, pln->setlayouts[i], NULL);
    free(pln->setlayouts);

    free(pln);
}

void
ear_vk_bind_pipeline(
    ear_vk_pipeline* pln
    ) {
    vkCmdBindPipeline(_ear_vk_comm_buffers[_ear_vk_cur_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pln->pipeline);
}
