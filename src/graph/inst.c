#include "graph/inst.h"
#include "graph/inst/ext.h"
#include <stdio.h>
#include <stdlib.h>

//

static int check_version(uint32_t *target);

//

VkInstance inst_create(void) {
    VkInstance ret = NULL;

    uint32_t target_ver;
    if (check_version(&target_ver)) goto out_retn;

    uint32_t nlayers;
    const char **layers;
    if (get_layers(&nlayers, &layers)) goto out_retn;
    if (check_layers(nlayers, layers)) goto out_free_layers;
    uint32_t nexts;
    const char **exts;
    if (get_exts(&nexts, &exts)) goto out_free_layers;
    if (check_exts(nexts, exts)) goto out_free_exts;

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
        .enabledLayerCount = nlayers,
        .ppEnabledLayerNames = layers,
        .enabledExtensionCount = nexts,
        .ppEnabledExtensionNames = exts,
    };
    VkInstance inst;
    VkResult r = vkCreateInstance(&ic_info, NULL, &inst);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkCreateInstance: ");
        vulk_err_str(stderr, r);
        goto out_free_exts;
    }
    ret = inst;

out_free_exts:
    free(exts); nexts = 0; exts = NULL;
out_free_layers:
    free(layers); nlayers = 0; layers = NULL;
out_retn:
    return ret;
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
