#include "graph/inst.h"
#include "graph/verr.h"
#include <stdio.h>

//

static int check_version(uint32_t *target);

//

VkInstance inst_create(GLFWwindow *win) {
    (void) win;

    uint32_t target_ver;
    if (check_version(&target_ver)) goto err_retn;

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = APPNAME,
        .applicationVersion =
            VK_MAKE_API_VERSION(0, APPVMAJOR, APPVMINOR, APPVPATCH),
        .pEngineName = NULL,
        .engineVersion = 0,
        .apiVersion = target_ver,
    };
    VkInstanceCreateInfo ic_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = NULL,
    };
    VkInstance inst;
    VkResult r = vkCreateInstance(&ic_info, NULL, &inst);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't create instance: ");
        vulk_err_str(stderr, r);
        goto err_retn;
    }
    return inst;

err_retn:
    return NULL;
}

void inst_destroy(VkInstance *inst) {
    vkDestroyInstance(*inst, NULL); *inst = NULL;
}

int check_version(uint32_t *target) {
    uint32_t ver;
    vkEnumerateInstanceVersion(&ver);
    *target = VK_API_VERSION_1_3;
    if (VK_API_VERSION_MAJOR(ver) != VK_API_VERSION_MAJOR(*target) ||
            VK_API_VERSION_MINOR(ver) < VK_API_VERSION_MINOR(*target) ||
            VK_API_VERSION_VARIANT(ver) != VK_API_VERSION_VARIANT(*target)) {
        fprintf(stderr, "Unsupported Vulkan version. "APPNAME" targets Vulkan %d.%d\n",
                VK_API_VERSION_MAJOR(*target), VK_API_VERSION_MINOR(*target));
        return 1;
    }
    return 0;
}
