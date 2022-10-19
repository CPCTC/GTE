#include "graph/dev.h"
#include "graph/dev/exts.h"
#include <stdlib.h>

VkDevice dev_init(VkInstance inst) {
    VkDevice ret = NULL;

    uint32_t grp;
    VkPhysicalDeviceGroupProperties *grps;
    if (select_pdev(inst, &grp, &grps)) goto out_retn;

    VkDeviceGroupDeviceCreateInfo grp_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .physicalDeviceCount = grps[grp].physicalDeviceCount,
        .pPhysicalDevices = grps[grp].physicalDevices,
    };

    VkDeviceCreateInfo dc_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &grp_info,
        .flags = 0,
        .queueCreateInfoCount = 0,
        .pQueueCreateInfos = NULL,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = NULL,
        .pEnabledFeatures = NULL,
    };

    VkDevice dev;
    VkResult r = vkCreateDevice(grps[grp].physicalDevices[0], &dc_info, NULL, &dev);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkCreateDevice: ");
        vulk_err_str(stderr, r);
	    goto out_free_grps;
    }
    ret = dev;

out_free_grps:
    free(grps); grps = NULL;
out_retn:
    return ret;
}

void dev_destroy(VkDevice *dev) {
    vkDestroyDevice(*dev, NULL); *dev = NULL;
}
