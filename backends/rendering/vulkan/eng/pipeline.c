#include "pipeline.h"
#include "cutil.h"

#include "../util/shader_mod.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "../init/device_log.h"
#include "../util/pipeline.h"
#include "../util/texture.h"
#include "framebuffer.h"

ear_vk_pipeline* _ear_vk_cur_pipeline;

void*
ear_vk_create_pipeline(
    ear_pipeline_desc desc
    ) {
    ear_vk_pipeline* pln = malloc(sizeof(ear_vk_pipeline));
    pln->depth = desc.depth;

    VkShaderModule vertex   = _ear_vk_create_shader_module(
        (const uint32_t*)desc.vertex.source,   desc.vertex.source_size);
    VkShaderModule fragment = _ear_vk_create_shader_module(
        (const uint32_t*)desc.fragment.source, desc.fragment.source_size);

    VkPipelineShaderStageCreateInfo stages[] = {
        _ear_vk_shader_module_info(vertex,   EAR_VK_SHADER_VERTEX),
        _ear_vk_shader_module_info(fragment, EAR_VK_SHADER_FRAGMENT),
        };
    
    VkPipelineInputAssemblyStateCreateInfo inputassembly = _ear_vk_make_pln_inputass(desc);
    VkPipelineViewportStateCreateInfo viewportstate = _ear_vk_make_pln_viewport();
    VkPipelineRasterizationStateCreateInfo rasterizer = _ear_vk_make_pln_rasterizer(desc);
    VkPipelineMultisampleStateCreateInfo multisampling = _ear_vk_make_pln_multisample();

    VkPipelineColorBlendAttachmentState colorblendattach;
    VkPipelineColorBlendStateCreateInfo colorblend = _ear_vk_make_pln_blend(desc, &colorblendattach);

    VkDynamicState dstates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        };
    VkPipelineDynamicStateCreateInfo dstate = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .dynamicStateCount = 2,
        .pDynamicStates    = dstates,
        };

    VkVertexInputBindingDescription* bindingdescs; VkVertexInputAttributeDescription* attribdescs;
    VkPipelineVertexInputStateCreateInfo vertexinputinfo = _ear_vk_make_pln_vertexinput(
        desc,
        &bindingdescs,
        &attribdescs
        );

    _ear_vk_make_pln_layout(pln, desc);

    VkFormat* colfmts = malloc(sizeof(VkFormat) * desc.color_fmt_amt);
    for (uint32_t i = 0; i < desc.color_fmt_amt; ++i)
        colfmts[i] = _ear_vk_convert_tex_fmt(desc.color_fmts[i]);

    VkPipelineRenderingCreateInfo renderinfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .pNext = NULL,

        .viewMask = 0,

        .colorAttachmentCount    = desc.color_fmt_amt,
        .pColorAttachmentFormats = colfmts,

        .depthAttachmentFormat   = VK_FORMAT_D24_UNORM_S8_UINT,
        .stencilAttachmentFormat = VK_FORMAT_D24_UNORM_S8_UINT,
        };

    VkPipelineDepthStencilStateCreateInfo dsstate = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext = NULL,

        .flags = 0,

        .depthTestEnable = true,
        .depthWriteEnable = true,
        .depthCompareOp = VK_COMPARE_OP_LESS,

        .depthBoundsTestEnable = false,
        .stencilTestEnable = false,
        };

    VkGraphicsPipelineCreateInfo createinfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = &renderinfo,

        .flags = 0,

        .stageCount = 2,
        .pStages    = stages,

        .pVertexInputState   = &vertexinputinfo,
        .pInputAssemblyState = &inputassembly,
        .pTessellationState  = NULL,
        .pViewportState      = &viewportstate,
        .pRasterizationState = &rasterizer,
        .pMultisampleState   = &multisampling,
        .pDepthStencilState  = &dsstate,
        .pColorBlendState    = &colorblend,
        .pDynamicState       = &dstate,

        .layout = pln->layout,

        .renderPass = NULL,
        .subpass    = 0,

        .basePipelineHandle = NULL,
        .basePipelineIndex  = -1,
        };

    eat_assert(vkCreateGraphicsPipelines(_ear_vk_device, NULL, 1, &createinfo, NULL, &pln->pipeline) == VK_SUCCESS, 
        "failed to create graphics pipeline!");

    if (!pln->depth) {
        VkPipelineRenderingCreateInfo renderinfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
            .pNext = NULL,

            .viewMask = 0,

            .colorAttachmentCount    = desc.color_fmt_amt,
            .pColorAttachmentFormats = colfmts,

            .depthAttachmentFormat   = VK_FORMAT_UNDEFINED,
            .stencilAttachmentFormat = VK_FORMAT_UNDEFINED,
            };

        VkGraphicsPipelineCreateInfo createinfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = &renderinfo,

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

            .renderPass = NULL,
            .subpass    = 0,

            .basePipelineHandle = NULL,
            .basePipelineIndex  = -1,
            };

        eat_assert(vkCreateGraphicsPipelines(_ear_vk_device, NULL, 1, &createinfo, NULL, &pln->nodepth) == VK_SUCCESS, 
            "failed to create graphics pipeline!");
    }

    _ear_vk_delete_shader_module(vertex);
    _ear_vk_delete_shader_module(fragment);

    free(bindingdescs);
    free(attribdescs);
    free(colfmts);

    return pln;
}
void
ear_vk_delete_pipeline(
    void* _pln
    ) {
    ear_vk_pipeline* pln = _pln;

    _ear_vk_device_wait_idle();

    if (!pln->depth)
        vkDestroyPipeline(_ear_vk_device, pln->nodepth, NULL);
    vkDestroyPipeline(_ear_vk_device, pln->pipeline, NULL);
    vkDestroyPipelineLayout(_ear_vk_device, pln->layout, NULL);

    free(pln->chips);
    free(pln);
}

void
ear_vk_bind_pipeline(
    void* _pln
    ) {
    ear_vk_pipeline* pln = _pln;

    if (_ear_vk_last_fb_depth)
        vkCmdBindPipeline(_ear_vk_comm_buffers[_ear_vk_cur_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pln->pipeline);
    else if (!pln->depth)
        vkCmdBindPipeline(_ear_vk_comm_buffers[_ear_vk_cur_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pln->nodepth);

    _ear_vk_cur_pipeline = pln;
}
