#pragma once

#include "graph/glfw.h"
#include "graph/pdev.h"

typedef enum {
    SCH_FAIL = -1,
    SCH_SUCCESS = 0,
    SCH_AGAIN = 1,
} Sch_status;

Sch_status sch_init(VkDevice dev, VkSurfaceKHR srf, const Surface_info *srfinfo,
        VkSwapchainKHR *sch);
void sch_destroy(VkDevice dev, VkSwapchainKHR *sch);
