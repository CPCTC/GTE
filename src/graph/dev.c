#include "graph/dev.h"
#include "graph/dev_ext.h"
#include <stdlib.h>

VkDevice dev_init(VkPhysicalDevice pdev, Queue_infos q_infos, Queues queues) {
    Dev_features feats;
    dev_features_init(&feats);
    for (uint32_t i = 0; i < NDEV_FEATURE_NAMES; i++)
        *(VkBool32 *) (((unsigned char *) &feats) + DEV_FEATURE_NAMES[i].off) = VK_TRUE;

    VkDeviceCreateInfo dc_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &feats.v1_0,
        .flags = 0,
        .queueCreateInfoCount = q_infos.ncreates,
        .pQueueCreateInfos = q_infos.creates,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = NDEV_EXTENSIONS,
        .ppEnabledExtensionNames = DEV_EXTENSIONS,
        .pEnabledFeatures = NULL,
    };

    VkDevice dev;
    VkResult r = vkCreateDevice(pdev, &dc_info, NULL, &dev);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkCreateDevice: ");
        vulk_err(stderr, r);
        return NULL;
    }
    for (Queue_name i = 0; i < MAX_Q; i++)
        vkGetDeviceQueue(dev,
                q_infos.creates[q_infos.by_q[i]].queueFamilyIndex,
                0, queues + i);
    return dev;
}

void dev_destroy(VkDevice *dev, Queues queues) {
    for (Queue_name q = 0; q < MAX_Q; q++)
        queues[q] = NULL;
    vkDestroyDevice(*dev, NULL); *dev = NULL;
}
