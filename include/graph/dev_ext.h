#pragma once

#include "graph/glfw.h"
#include <stddef.h>
#include <stdint.h>

extern const uint32_t NDEV_EXTENSIONS;
extern const char * const * const DEV_EXTENSIONS;

typedef struct {
    const char *name;
    size_t off;
} Dev_feature_name;
extern const uint32_t NDEV_FEATURE_NAMES;
extern const Dev_feature_name *DEV_FEATURE_NAMES;

typedef struct {
    VkPhysicalDeviceFeatures2 v1_0;
    VkPhysicalDeviceVulkan13Features v1_3;
} Dev_features;
void dev_features_init(Dev_features *feats);
