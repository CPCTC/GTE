#pragma once

#include "graph/glfw.h"
#include "graph/img.h"

typedef struct {
    uint32_t nframes;
    VkFramebuffer *frames;
} Frames;

int frames_init(VkDevice dev,
        const Images *imgs, VkRenderPass rpass, VkExtent2D ext,
        Frames *frames);

void frames_destroy(VkDevice dev, Frames *frames);
