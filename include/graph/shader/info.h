#pragma once

#include "graph/glfw.h"

typedef struct {
    VkShaderStageFlagBits stage;
    const char *start;
    const char *end;
} Shader_info;

typedef struct {
    uint32_t nshaders;
    const Shader_info *shaders;
} Shaders_info;
