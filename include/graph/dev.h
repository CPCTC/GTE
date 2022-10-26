#pragma once

#include "graph/glfw.h"
#include "graph/pdev.h"

VkDevice dev_init(VkPhysicalDevice pdev, Queue_infos q_infos, Queues queues);
void dev_destroy(VkDevice *dev, Queues queues);
