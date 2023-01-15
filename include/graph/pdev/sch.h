#pragma once

#include "graph/glfw.h"
#include <stdbool.h>

int check_dev_prmodes(VkPhysicalDevice pdev, VkSurfaceKHR srf, bool *works);
