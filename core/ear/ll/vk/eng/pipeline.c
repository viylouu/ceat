#include "pipeline.h"
#include "../../../../cutil.h"

#include "../sc/render_pass.h"
#include "shader_mod.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "../init/device_log.h"

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

    VkPipelineVertexInputStateCreateInfo vertexinputinfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions    = NULL,

        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions    = NULL,
        };

    VkPipelineInputAssemblyStateCreateInfo inputassembly = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
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

        .depthClampEnable = false,

        .rasterizerDiscardEnable = false,

        .polygonMode = VK_POLYGON_MODE_FILL,

        .lineWidth = 1,

        .cullMode  = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,

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

        .blendEnable = false,

        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp        = VK_BLEND_OP_ADD,

        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp        = VK_BLEND_OP_ADD,
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

    VkPipelineLayoutCreateInfo layoutinfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .setLayoutCount = 0,
        .pSetLayouts    = NULL,

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

    return pln;
}
void
ear_vk_delete_pipeline(
    ear_vk_pipeline* pln
    ) {
    _ear_vk_device_wait_idle();

    vkDestroyPipeline(_ear_vk_device, pln->pipeline, NULL);
    vkDestroyPipelineLayout(_ear_vk_device, pln->layout, NULL);

    free(pln);
}

void
ear_vk_bind_pipeline(
    ear_vk_pipeline* pln
    ) {
    vkCmdBindPipeline(_ear_vk_comm_buffers[_ear_vk_cur_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pln->pipeline);
}
