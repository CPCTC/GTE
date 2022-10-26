#pragma once

#include "graph/glfw.h"
#include "graph/dev/pdev.h"

int create_queues(VkPhysicalDevice pdev, VkSurfaceKHR srf, Queue_infos *qs);
