#pragma once

#include "graph/glfw.h"
#include "graph/sch.h"

typedef struct {
    uint32_t nimgs;
    VkImage *imgs;
    VkImageView *views;
} Images;

int img_init(VkDevice dev, VkSwapchainKHR sch, Surface_info srf_info, Images *imgs);
void img_destroy(VkDevice dev, Images *imgs);
