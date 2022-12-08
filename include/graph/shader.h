#pragma once

#include "graph/glfw.h"

typedef struct {
    uint32_t nshaders;
    VkShaderModule *mods;
    VkShaderStageFlagBits *stages;
} Shaders;

int shaders_init(VkDevice dev, Shaders *shaders);
void shaders_destroy(VkDevice dev, Shaders *shaders);
