#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>

const char *glfw_err_str(void);
void vulk_err_str(FILE *stream, VkResult r);
