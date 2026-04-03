#include "pipeline.h"
#include "../../../../cutil.h"

#include "../../pipeline.h"

#include "../sc/render_pass.h"
#include "../../buffer.h"
#include "bindset.h"
#include "../util/shader_mod.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "../init/device_log.h"
#include "../util/pipeline.h"

ear_vk_pipeline* _ear_vk_cur_pipeline;

ear_vk_pipeline*
ear_vk_create_pipeline(
    ear_pipeline_desc desc
    ) {
    ear_vk_pipeline* pln = malloc(sizeof(ear_vk_pipeline));

    VkShaderModule vertex   = _ear_vk_create_shader_module((const uint32_t*)desc.vertex.source,   desc.vertex.source_size);
    VkShaderModule fragment = _ear_vk_create_shader_module((const uint32_t*)desc.fragment.source, desc.fragment.source_size);

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

    pln->chips = malloc(sizeof(VkDescriptorSetLayout) * desc.bindset_amt);
    pln->chip_amt = desc.bindset_amt;
    for (uint32_t i = 0; i < desc.bindset_amt; ++i)
        pln->chips[i] = ((ear_vk_bindset*)desc.bindsets[i]->vk)->lay;

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

    //for (uint32_t i = 0; i < pln->chip_amt; ++i)
    //    vkDestroyDescriptorSetLayout(_ear_vk_device, pln->chips[i], NULL);
    free(pln->chips);

    free(pln);
}

void
ear_vk_bind_pipeline(
    ear_vk_pipeline* pln
    ) {
    vkCmdBindPipeline(_ear_vk_comm_buffers[_ear_vk_cur_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pln->pipeline);

    _ear_vk_cur_pipeline = pln;
}
