#include "graph/dev_ext.h"

static const char * const exts[] = {
    "VK_KHR_swapchain",
};
const char * const * const DEV_EXTENSIONS = exts;
const uint32_t NDEV_EXTENSIONS = sizeof (exts) / sizeof (exts[0]);
