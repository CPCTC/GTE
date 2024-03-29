#include "graph/pdev.h"
#include "graph/pdev/ext.h"
#include "graph/pdev/q.h"
#include "graph/pdev/srf.h"
#include "graph/dev_ext.h"
#include "graph/ver.h"
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//

static int rate_dev(VkPhysicalDevice dev, VkSurfaceKHR srf,
        uint32_t *score, Surface_info *srfinfo, Queue_infos *qs);

//

VkPhysicalDevice select_pdev(VkInstance inst, VkSurfaceKHR srf,
        Surface_info *srfinfo, Queue_infos *qs) {
    VkPhysicalDevice ret = NULL;

    uint32_t ndevs;
    VkResult r = vkEnumeratePhysicalDevices(inst, &ndevs, NULL);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumeratePhysicalDevices: ");
        vulk_err(stderr, r);
        goto out_retn;
    }
    VkPhysicalDevice *devs = malloc(ndevs * sizeof (VkPhysicalDevice));
    if (!devs) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto out_retn;
    }
    r = vkEnumeratePhysicalDevices(inst, &ndevs, devs);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumeratePhysicalDevices: ");
        vulk_err(stderr, r);
        goto out_free_devs;
    }

    uint32_t best_dev = 0;
    uint32_t best_score = 0;
    for (uint32_t i = 0; i < ndevs; i++) {
        Surface_info dev_srfinfo;
        Queue_infos dev_qs;
        uint32_t score;
        if (rate_dev(devs[i], srf, &score, &dev_srfinfo, &dev_qs))
            goto out_free_devs;
        if (score > best_score) {
            best_dev = i;
            best_score = score;
            *srfinfo = dev_srfinfo;
            *qs = dev_qs;
        }
    }

    if (!best_score) {
        fprintf(stderr, "No suitable Vulkan device found.\n");
        goto out_free_devs;
    }

    ret = devs[best_dev];

out_free_devs:
    free(devs);
out_retn:
    return ret;
}

int rate_dev(VkPhysicalDevice dev, VkSurfaceKHR srf,
        uint32_t *score, Surface_info *srfinfo, Queue_infos *qs) {
    *score = 0;

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(dev, &props);

    if (check_version(props.apiVersion)) {
        fprintf(stderr, "Device %s: ", props.deviceName);
        version_warning(stderr, props.apiVersion);
        return 0;
    }

    bool works;
    if (check_dev_exts(dev, NDEV_EXTENSIONS, DEV_EXTENSIONS, &works)) return 1;
    if (!works) return 0;

    check_dev_features(dev, &works);
    if (!works) return 0;

    if (check_srf(dev, srf, srfinfo, &works)) return 1;
    if (!works) return 0;

    if (create_queues(dev, srf, qs, &works)) return 1;
    if (!works) return 0;

    *score = 1;
    switch (props.deviceType) {
        default:
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            score += 10;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            score += 11;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            score += 12;
            break;
    }

    return 0;
}
