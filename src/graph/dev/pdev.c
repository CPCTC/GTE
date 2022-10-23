#include "graph/dev/pdev.h"
#include "graph/dev/q.h"
#include "graph/ver.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//

static int pdev_glance(VkInstance inst);
static uint32_t rate_group(VkInstance inst, VkPhysicalDeviceGroupProperties grp, Queue_infos *qs);

//

int select_pdev(VkInstance inst, uint32_t *grp, VkPhysicalDeviceGroupProperties **grps, Queue_infos *qs) {
    if (pdev_glance(inst)) goto err_retn;
    uint32_t ngrps;
    VkResult r = vkEnumeratePhysicalDeviceGroups(inst, &ngrps, NULL);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumeratePhysicalDeviceGroups: ");
        vulk_err(stderr, r);
        goto err_retn;
    }
    *grps = malloc(ngrps * sizeof (VkPhysicalDeviceGroupProperties));
    if (!*grps) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto err_retn;
    }
    for (uint32_t i = 0; i < ngrps; i++) {
        (*grps)[i].sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GROUP_PROPERTIES;
        (*grps)[i].pNext = NULL;
    }
    r = vkEnumeratePhysicalDeviceGroups(inst, &ngrps, *grps);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumeratePhysicalDeviceGroups: ");
        vulk_err(stderr, r);
        goto err_free_groups;
    }

    uint32_t best_grp = 0;
    uint32_t best_score = 0;
    for (uint32_t i = 0; i < ngrps; i++) {
        Queue_infos grp_qs;
        uint32_t score = rate_group(inst, (*grps)[i], &grp_qs);
        if (score > best_score) {
            best_grp = i;
            best_score = score;
	    *qs = grp_qs;
        }
    }

    if (!best_score) {
        fprintf(stderr, "No suitable Vulkan device found.\n");
        goto err_free_groups;
    }

    *grp = best_grp;
    return 0;

err_free_groups:
    free(*grps);
err_retn:
    return 1;
}

// Workaround validation layer bug.
// I get all my pdev handles via vkEnumeratePhysicalDeviceGroups,
// but the validation layer thinks pdevs can only come from vkEnumeratePhysicalDevices.
int pdev_glance(VkInstance inst) {
    int ret = 1;
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

    // Do literally nothing with this information

    ret = 0;

out_free_devs:
    free(devs);
out_retn:
    return ret;
}

uint32_t rate_group(VkInstance inst, VkPhysicalDeviceGroupProperties grp, Queue_infos *qs) {
    uint32_t score = 1;
    VkPhysicalDeviceProperties2 props2 = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
        .pNext = NULL,
    };
    vkGetPhysicalDeviceProperties2(grp.physicalDevices[0], &props2);
    VkPhysicalDeviceProperties props = props2.properties;

    if (check_version(props.apiVersion)) return 0;
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
    score *= grp.physicalDeviceCount;
    if (create_queues(inst, grp.physicalDevices[0], qs)) return 0;
    return score;
}
