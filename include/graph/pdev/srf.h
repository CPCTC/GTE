#pragma once

#include "graph/glfw.h"
#include "graph/pdev.h"
#include <stdbool.h>

int check_srf(VkPhysicalDevice pdev, VkSurfaceKHR srf,
        Surface_info *srfinfo, bool *works);
