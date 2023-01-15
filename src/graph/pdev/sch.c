#include "graph/pdev/sch.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int check_dev_prmodes(VkPhysicalDevice pdev, VkSurfaceKHR srf, bool *works) {
    int ret = 1;

    uint32_t nmodes;
    VkResult r = vkGetPhysicalDeviceSurfacePresentModesKHR(pdev, srf, &nmodes, NULL);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkGetPhysicalDeviceSurfacePresentModesKHR: ");
        vulk_err(stderr, r);
        goto out_retn;
    }
    VkPresentModeKHR *modes = malloc(nmodes * sizeof (VkPresentModeKHR));
    if (!modes) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto out_retn;
    }
    r = vkGetPhysicalDeviceSurfacePresentModesKHR(pdev, srf, &nmodes, modes);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkGetPhysicalDeviceSurfacePresentModesKHR: ");
        vulk_err(stderr, r);
        goto out_free;
    }

    ret = 0;

    for (uint32_t i = 0; i < nmodes; i++)
        if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            *works = 1;
            goto out_free;
        }

    *works = 0;

out_free:
    free(modes);
out_retn:
    return ret;
}
