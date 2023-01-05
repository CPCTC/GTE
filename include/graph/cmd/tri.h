#pragma once

#include "graph/glfw.h"
#include "graph/pdev.h"
#include "graph/pool.h"

typedef struct {
    VkCommandBuffer draw;
    //VkCommandBuffer present;
} Triangle_cmds;

int triangle_cmds_init(VkDevice dev,
        const Queue_infos *q_infos, const Pools *pools,
        Triangle_cmds *tri_cmds);

void triangle_cmds_destroy(VkDevice dev,
        const Queue_infos *q_infos, const Pools *pools,
        Triangle_cmds *tri_cmds);
