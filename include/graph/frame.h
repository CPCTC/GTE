#pragma once

#include "graph/glfw.h"
#include "graph/img.h"

int frames_init(VkDevice dev,
        const Images *imgs, VkRenderPass rpass, VkExtent2D ext,
        VkFramebuffer **frames);
void frames_destroy(VkDevice dev, const Images *imgs, VkFramebuffer **frames);
