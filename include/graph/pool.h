#pragma once

#include "graph/glfw.h"
#include "graph/pdev.h"
#include <stdbool.h>

typedef struct {
    bool used[MAX_Q];
    VkCommandPool pools[MAX_Q]; // sparse; indexed by family
} Pools;

int pools_init(VkDevice dev, const Queue_infos *q_infos, Pools *pools);
void pools_destroy(VkDevice dev, Pools *pools);
