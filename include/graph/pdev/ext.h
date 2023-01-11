#pragma once

#include "graph/glfw.h"
#include <stdbool.h>
#include <stdint.h>

int check_dev_exts(VkPhysicalDevice pdev, uint32_t nexts, const char * const *exts, bool *works);
void check_dev_features(VkPhysicalDevice dev, bool *works);
