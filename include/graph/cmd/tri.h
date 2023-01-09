#pragma once

#include "graph/frame.h"
#include "graph/glfw.h"
#include "graph/pdev.h"
#include "graph/pool.h"

typedef struct {
    uint32_t nbufs;
    VkCommandBuffer *bufs; // indexed by frame/image index
} Triangle_cmds;

int triangle_cmds_init(VkDevice dev, const Pools *pools, VkRenderPass rpass,
        const Frames *frames, VkPipeline pipe, VkExtent2D ext,
        Triangle_cmds *cmds);

void triangle_cmds_destroy(VkDevice dev, const Pools *pools, Triangle_cmds *cmds);
