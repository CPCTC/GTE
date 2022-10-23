#pragma once

#include "graph/glfw.h"
#include "graph/dev/pdev.h"

int create_queues(VkInstance inst, VkPhysicalDevice pdev, Queue_infos *qs);
