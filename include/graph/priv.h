#pragma once

#include "graph.h"
#include "graph/cmd/tri.h"
#include "graph/frame.h"
#include "graph/glfw.h"
#include "graph/img.h"
#include "graph/pdev.h"
#include "graph/pool.h"
#include "graph/sch.h"
#include "graph/shader.h"
#include "graph/sync.h"

typedef struct {
    GLFWwindow *win;
    VkInstance inst;
    VkDebugUtilsMessengerEXT msgr;
    VkSurfaceKHR srf;
    VkPhysicalDevice pdev;
    Queue_infos q_infos;
    VkDevice dev;
    Queues qs;
    VkSwapchainKHR sch;
    Surface_info srf_info;
    Images imgs;
    Shaders shaders;
    VkRenderPass rpass;
    VkPipelineLayout tri_layout;
    VkPipeline tri_pipe;
    Frames frames;
    Pools pools;
    Triangle_cmds tri_cmds;
    Sync sync;
} Graph;
