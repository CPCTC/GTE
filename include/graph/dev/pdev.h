#pragma once

#include "graph/glfw.h"
#include "graph/dev.h"

typedef struct {
	uint32_t ncreates;
	VkDeviceQueueCreateInfo creates[MAX_Q];
	uint32_t fams[MAX_Q];
} Queue_infos;

int select_pdev(VkInstance inst, uint32_t *grp, VkPhysicalDeviceGroupProperties **grps, Queue_infos *qs);
