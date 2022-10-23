#pragma once

#include "graph/glfw.h"

GLFWwindow *win_open(void);
void win_close(GLFWwindow **w);

VkSurfaceKHR srf_init(VkInstance inst, GLFWwindow *win);
void srf_destroy(VkInstance inst, VkSurfaceKHR *srf);
