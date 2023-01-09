#pragma once

#include "graph/glfw.h"
#include "graph/pdev.h"

typedef struct {
    uint32_t npools;
    VkCommandPool pools[MAX_Q]; // indexed via by_q values
    uint32_t by_q[MAX_Q]; // indexed by queue name
} Pools;

int pools_init(VkDevice dev, const Queue_infos *q_infos, Pools *pools);
void pools_destroy(VkDevice dev, Pools *pools);
