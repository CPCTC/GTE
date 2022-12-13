#pragma once

#include "graph/glfw.h"
#include "graph/shader.h"

void stage_infos_init(const Shaders *shaders,
        uint32_t nstages, const uint32_t *names,
        VkPipelineShaderStageCreateInfo *infos);
