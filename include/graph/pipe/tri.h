#pragma once

#include "graph/glfw.h"
#include "graph/shader.h"

int triangle_pipe_init(VkDevice dev, const Shaders *shaders,
        VkRenderPass rpass, uint32_t subpass,
        VkExtent2D view_extent,
        VkPipelineLayout *layout, VkPipeline *pipe);
void triangle_pipe_destroy(VkDevice dev, VkPipelineLayout *layout, VkPipeline *pipe);
