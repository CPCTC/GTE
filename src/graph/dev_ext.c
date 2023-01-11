#include "graph/dev_ext.h"
#include "graph/glfw.h"

static const char * const exts[] = {
    "VK_KHR_swapchain",
};
const uint32_t NDEV_EXTENSIONS = sizeof (exts) / sizeof (exts[0]);
const char * const * const DEV_EXTENSIONS = exts;

#define f(name) {#name, (size_t) &((Dev_features *) 0)->name}
static const Dev_feature_name feats[] = {
    f(v1_3.synchronization2),
};
#undef f
const uint32_t NDEV_FEATURE_NAMES = sizeof (feats) / sizeof (feats[0]);
const Dev_feature_name *DEV_FEATURE_NAMES = feats;

void dev_features_init(Dev_features *feats) {
    *feats = (Dev_features) {
        .v1_0 = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &feats->v1_3,
        },
        .v1_3 = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .pNext = NULL,
        },
    };
}
