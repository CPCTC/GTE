#pragma once

#include "graph/glfw.h"
#include "graph/pdev.h"
#include "graph/pool.h"

int triangle_cmds_init(VkDevice dev, const Pools *pools, VkRenderPass rpass,
        uint32_t nframes, VkFramebuffer *frames,
        VkPipeline pipe, VkExtent2D ext,
        VkCommandBuffer **cmds);

void triangle_cmds_destroy(VkDevice dev, const Pools *pools,
        uint32_t ncmds, VkCommandBuffer **cmds);
