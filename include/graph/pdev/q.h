#pragma once

#include "graph/glfw.h"
#include "graph/pdev.h"
#include <stdbool.h>

int create_queues(VkPhysicalDevice pdev, VkSurfaceKHR srf, Queue_infos *qs, bool *works);
