#include "pipeline.h"
#include "cutil.h"

//#include "../sc/render_pass.h"
#include "../util/shader_mod.h"
#include "../init/comm_buffer.h"
#include "../sc/swapchain.h"
#include "../init/device_log.h"
#include "../util/pipeline.h"
//#include "framebuffer.h"
#include "../util/texture.h"

ear_vk_pipeline* _ear_vk_cur_pipeline;

ear_vk_pipeline*
ear_vk_create_pipeline(
    ear_pipeline_desc desc
    ) {
    ear_vk_pipeline* pln = malloc(sizeof(ear_vk_pipeline));

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

        .depthAttachmentFormat = desc.depth? _ear_vk_convert_tex_fmt(desc.depth_fmt) : VK_FORMAT_UNDEFINED,
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

    eat_assert(vkCreateGraphicsPipelines(_ear_vk_device, NULL, 1, &createinfo, NULL, &pln->pipeline) == VK_SUCCESS, 
        "failed to create graphics pipeline!");

    _ear_vk_delete_shader_module(vertex);
    _ear_vk_delete_shader_module(fragment);

    free(bindingdescs);
    free(attribdescs);
    free(colfmts);

    return pln;
}
void
ear_vk_delete_pipeline(
    ear_vk_pipeline* pln
    ) {
    _ear_vk_device_wait_idle();

    vkDestroyPipeline(_ear_vk_device, pln->pipeline, NULL);
    vkDestroyPipelineLayout(_ear_vk_device, pln->layout, NULL);

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
