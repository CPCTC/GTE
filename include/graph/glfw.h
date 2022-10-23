#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>

#define FP_IMPL(a,b) a ## b
#define fpi(inst,name) ((FP_IMPL(PFN_,name)) vkGetInstanceProcAddr(inst, #name))
#define fpd(dev,name) ((FP_IMPL(PFN_,name)) vkGetDeviceProcAddr(dev, #name))

const char *glfw_err_str(void);
void vulk_err(FILE *stream, VkResult r);
void vulk_err_str(FILE *stream, VkResult r);
