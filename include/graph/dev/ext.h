#pragma once

#include "graph/glfw.h"
#include <stdint.h>

extern const uint32_t NDEV_EXTENSIONS;
extern const char * const * const DEV_EXTENSIONS;

int check_dev_exts(VkPhysicalDevice pdev, uint32_t nexts, const char * const *exts);
