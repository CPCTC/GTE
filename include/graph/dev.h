#pragma once

#include "graph/glfw.h"

VkDevice dev_init(VkInstance inst);
void dev_destroy(VkDevice *dev);
