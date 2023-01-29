#pragma once

#include "graph/glfw.h"
#include <stdint.h>

typedef struct {
    uint32_t nimages;
    VkSurfaceFormatKHR fmt;
    VkExtent2D extent;
    VkSurfaceTransformFlagBitsKHR xform;
    VkPresentModeKHR prmode;
} Surface_info;

typedef enum {
    GRAPHICS_Q = 0,
    PRESENT_Q,
    MAX_Q, // *cheering*
} Queue_name;
typedef VkQueue Queues[MAX_Q];

typedef struct {
	uint32_t ncreates;
	VkDeviceQueueCreateInfo creates[MAX_Q]; // indexed via by_q values.
	uint32_t by_q[MAX_Q]; // indexed by queue name
} Queue_infos;

VkPhysicalDevice select_pdev(VkInstance inst, VkSurfaceKHR srf,
        Surface_info *srfinfo, Queue_infos *qs);
