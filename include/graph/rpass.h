#pragma once
#include "graph/glfw.h"

int rpass_init(VkDevice dev, VkFormat fmt, VkRenderPass *rpass);
void rpass_destroy(VkDevice dev, VkRenderPass *rpass);
