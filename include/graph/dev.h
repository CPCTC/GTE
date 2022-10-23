#pragma once

#include "graph/glfw.h"

typedef enum {
    GRAPHICS_Q = 0,
    PRESENT_Q,
    MAX_Q, // *cheering*
} Queue_name;
typedef VkQueue Queues[MAX_Q];

VkDevice dev_init(VkInstance inst, Queues queues);
void dev_destroy(VkDevice *dev, Queues queues);
