#include "graph/inst.h"
#include "graph/inst/ext.h"
#include "graph/debug.h"
#include "graph/ver.h"
#include <stdio.h>
#include <stdlib.h>

//

VkInstance inst_create(void) {
    VkInstance ret = NULL;

    uint32_t ver;
    VkResult r = vkEnumerateInstanceVersion(&ver);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumerateInstanceVersion: ");
        vulk_err(stderr, r);
        goto out_retn;
    }
    if (check_version(ver)) {
        version_warning(stderr, ver);
        goto out_retn;
    }

    uint32_t nlayers;
    const char **layers;
    if (get_layers(&nlayers, &layers)) goto out_retn;
    if (check_layers(nlayers, layers)) goto out_free_layers;
    uint32_t nexts;
    const char **exts;
    if (get_exts(&nexts, &exts)) goto out_free_layers;
    if (check_exts(nexts, exts)) goto out_free_exts;

#ifdef DEBUG
    VkDebugUtilsMessengerCreateInfoEXT db_info = debug_make_msgr_info(NULL);
#endif

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = APPNAME,
        .applicationVersion =
            VK_MAKE_API_VERSION(0, APPVMAJOR, APPVMINOR, APPVPATCH),
        .pEngineName = NULL,
        .engineVersion = 0,
        .apiVersion = TARGET_VULK_VER,
    };
    VkInstanceCreateInfo ic_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#ifdef DEBUG
        .pNext = &db_info,
#else
        .pNext = NULL,
#endif
        .flags = 0,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = nlayers,
        .ppEnabledLayerNames = layers,
        .enabledExtensionCount = nexts,
        .ppEnabledExtensionNames = exts,
    };
    VkInstance inst;
    r = vkCreateInstance(&ic_info, NULL, &inst);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkCreateInstance: ");
        vulk_err(stderr, r);
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
