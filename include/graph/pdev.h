#pragma once

#include "graph/glfw.h"
#include <stdint.h>

typedef enum {
    GRAPHICS_Q = 0,
    PRESENT_Q,
    MAX_Q, // *cheering*
} Queue_name;
typedef VkQueue Queues[MAX_Q];

typedef struct {
	uint32_t ncreates;
	VkDeviceQueueCreateInfo creates[MAX_Q];
	uint32_t fams[MAX_Q];
} Queue_infos;

VkPhysicalDevice select_pdev(VkInstance inst, VkSurfaceKHR srf, Queue_infos *qs);
