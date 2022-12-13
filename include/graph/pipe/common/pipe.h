#pragma once

#include "graph/glfw.h"

int pipe_init(VkDevice dev, VkGraphicsPipelineCreateInfo *info, VkPipeline *pipe);
void pipe_destroy(VkDevice dev, VkPipeline *pipe);
