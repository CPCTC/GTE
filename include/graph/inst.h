#pragma once
#include "graph/glfw.h"

VkInstance inst_create(GLFWwindow *win);
void inst_destroy(VkInstance *inst);
