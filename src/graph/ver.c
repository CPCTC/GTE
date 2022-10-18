#include "graph/ver.h"
#include "graph/glfw.h"
#include <stdio.h>

const uint32_t TARGET_VULK_VER = VK_API_VERSION_1_3;

int check_version(uint32_t ver) {
    if (VK_API_VERSION_MAJOR(ver) != VK_API_VERSION_MAJOR(TARGET_VULK_VER) ||
            VK_API_VERSION_MINOR(ver) < VK_API_VERSION_MINOR(TARGET_VULK_VER) ||
            VK_API_VERSION_VARIANT(ver) != VK_API_VERSION_VARIANT(TARGET_VULK_VER)) {
        fprintf(stderr, "Unsupported Vulkan version. "APPNAME" TARGET_VULK_VERs Vulkan %d.%d\n",
                VK_API_VERSION_MAJOR(TARGET_VULK_VER), VK_API_VERSION_MINOR(TARGET_VULK_VER));
        return 1;
    }
    return 0;
}
