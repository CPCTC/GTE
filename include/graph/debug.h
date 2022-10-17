#pragma once

#include "graph/glfw.h"

VkDebugUtilsMessengerEXT debug_start(VkInstance inst);
void debug_stop(VkInstance inst, VkDebugUtilsMessengerEXT *msgr);
VkDebugUtilsMessengerCreateInfoEXT debug_make_msgr_info(void *pNext);
