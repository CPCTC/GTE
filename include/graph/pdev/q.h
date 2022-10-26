#pragma once

#include "graph/glfw.h"
#include "graph/pdev.h"

int create_queues(VkPhysicalDevice pdev, VkSurfaceKHR srf, Queue_infos *qs);
