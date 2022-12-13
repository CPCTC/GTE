#include "graph/pipe/tri.h"
#include "graph/pipe/common/pipe.h"
#include "graph/pipe/common/shader.h"
#include "gen/shader.h"

int triangle_pipe_init(VkDevice dev, const Shaders *shaders,
        VkRenderPass rpass, uint32_t subpass,
        VkExtent2D view_extent,
        VkPipelineLayout *layout, VkPipeline *pipe) {
    // Stages

    uint32_t stage_names[] = {
        SHADER_vert_triangle,
        SHADER_frag_red,
    };
    const uint32_t nstages = sizeof (stage_names) / sizeof (stage_names[0]);
    VkPipelineShaderStageCreateInfo stages[nstages];
    stage_infos_init(shaders, nstages, stage_names, stages);

    // Vertex Input

    VkPipelineVertexInputStateCreateInfo vert_inp = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .vertexBindingDescriptionCount = 0,
        //.pVertexBindingDescriptions = ...
        .vertexAttributeDescriptionCount = 0,
        //.pVertexAttributeDescriptions = ...
    };

    // Input Assembly

    VkPipelineInputAssemblyStateCreateInfo inp_asm = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };

    // Tessellation

    // Viewport

    VkViewport view = {
        .x = 0,
        .y = 0,
        .width = view_extent.width,
        .height = view_extent.height,
        .minDepth = 0,
        .maxDepth = 0,
    };

    VkRect2D scissor = {
        .offset = {
            .x = 0,
            .y = 0,
        },
        .extent = view_extent,
    };

    VkPipelineViewportStateCreateInfo view_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .viewportCount = 1,
        .pViewports = &view,
        .scissorCount = 1,
        .pScissors = &scissor,
    };

    // Rasterization

    VkPipelineRasterizationStateCreateInfo raster = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .depthClampEnable = 0,
        .rasterizerDiscardEnable = 0, // Double negative >:(
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_NONE,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f,
    };

    // Multisample

    VkPipelineMultisampleStateCreateInfo msamp = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .pSampleMask = NULL,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,
    };

    // Depth Stencil

    // Color Blend

    VkPipelineColorBlendAttachmentState color_attach = {
        .blendEnable = VK_FALSE,
        //.srcColorBlendFactor = ...
        //.dstColorBlendFactor = ...
        //.colorBlendOp = ...
        //.srcAlphaBlendFactor = ...
        //.dstAlphaBlendFactor = ...
        //.alphaBlendOp = ...
        //.colorWriteMask = ...
    };

    VkPipelineColorBlendStateCreateInfo color = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        //.logicOp = ...
        .attachmentCount = 1,
        .pAttachments = &color_attach,
        //.blendConstants = ...
    };

    // Dynamic

    // Layout

    // TODO: Eventually, this should be generated from the shaders
    VkPipelineLayoutCreateInfo layout_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .setLayoutCount = 0,
        //.pSetLayouts = ...
        .pushConstantRangeCount = 0,
        //.pPushConstantRanges = ...
    };

    VkResult r = vkCreatePipelineLayout(dev, &layout_info, NULL, layout);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkCreatePipelineLayout: ");
        vulk_err(stderr, r);
        goto err_retn;
    }

    // Graphics Pipeline

    VkGraphicsPipelineCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .stageCount = nstages,
        .pStages = stages,
        .pVertexInputState = &vert_inp,
        .pInputAssemblyState = &inp_asm,
        //.pTessellationState = ...
        .pViewportState = &view_info,
        .pRasterizationState = &raster,
        .pMultisampleState = &msamp,
        .pDepthStencilState = NULL,
        .pColorBlendState = &color,
        .pDynamicState = NULL,
        .layout = *layout,
        .renderPass = rpass,
        .subpass = subpass,
        .basePipelineHandle = NULL,
        .basePipelineIndex = 0,
    };

    if (pipe_init(dev, &info, pipe)) goto err_free_layout;

    return 0;

err_free_layout:
    vkDestroyPipelineLayout(dev, *layout, NULL); *layout = NULL;
err_retn:
    *pipe = NULL;
    return 1;
}

void triangle_pipe_destroy(VkDevice dev, VkPipelineLayout *layout, VkPipeline *pipe) {
    pipe_destroy(dev, pipe);
    vkDestroyPipelineLayout(dev, *layout, NULL); *layout = NULL;
}
