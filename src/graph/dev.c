#include "graph/dev.h"
#include "graph/dev/pdev.h"
#include <stdlib.h>

VkDevice dev_init(VkInstance inst, Queues queues) {
    VkDevice ret = NULL;

    uint32_t grp;
    VkPhysicalDeviceGroupProperties *grps;
    Queue_infos q_infos;
    if (select_pdev(inst, &grp, &grps, &q_infos)) goto out_retn;

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
        .queueCreateInfoCount = q_infos.ncreates,
        .pQueueCreateInfos = q_infos.creates,
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
        vulk_err(stderr, r);
	    goto out_free;
    }
    for (uint32_t i = 0; i < MAX_Q; i++) {
        vkGetDeviceQueue(dev, q_infos.fams[i], 0, queues + i);
    }
    ret = dev;

out_free:
    free(grps); grps = NULL;
out_retn:
    return ret;
}

void dev_destroy(VkDevice *dev, Queues queues) {
    for (Queue_name q = 0; q < MAX_Q; q++)
        queues[q] = NULL;
    vkDestroyDevice(*dev, NULL); *dev = NULL;
}
