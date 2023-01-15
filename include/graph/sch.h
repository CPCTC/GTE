#pragma once

#include "graph/glfw.h"

typedef enum {
    SCH_FAIL = -1,
    SCH_SUCCESS = 0,
    SCH_AGAIN = 1,
} Sch_status;

typedef struct {
    uint32_t nimages;
    VkSurfaceFormatKHR fmt;
    VkExtent2D extent;
    VkSurfaceTransformFlagBitsKHR xform;
} Surface_info;

Sch_status sch_init(VkDevice dev, VkPhysicalDevice pdev, VkSurfaceKHR srf,
        VkSwapchainKHR *sch, Surface_info *srfinfo);
void sch_destroy(VkDevice dev, VkSwapchainKHR *sch);
