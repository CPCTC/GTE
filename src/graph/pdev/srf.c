#include "graph/pdev/srf.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//

static int check_srf_caps(VkPhysicalDevice pdev, VkSurfaceKHR srf,
        Surface_info *srfinfo, bool *works);
static int check_srf_fmts(VkPhysicalDevice pdev, VkSurfaceKHR srf,
        Surface_info *srfinfo, bool *works);
static int check_srf_prmodes(VkPhysicalDevice pdev, VkSurfaceKHR srf,
        Surface_info *srfinfo, bool *works);
static uint32_t clamp_1D(uint32_t low, uint32_t x, uint32_t high);
static VkExtent2D clamp_2D(VkExtent2D low, VkExtent2D x, VkExtent2D high);

//

int check_srf(VkPhysicalDevice pdev, VkSurfaceKHR srf,
        Surface_info *srfinfo, bool *works) {

    if (check_srf_caps(pdev, srf, srfinfo, works)) return 1;
    if (!*works) return 0;
    if (check_srf_fmts(pdev, srf, srfinfo, works)) return 1;
    if (!*works) return 0;
    if (check_srf_prmodes(pdev, srf, srfinfo, works)) return 1;

    return 0;
}

int check_srf_caps(VkPhysicalDevice pdev, VkSurfaceKHR srf,
        Surface_info *srfinfo, bool *works) {
    *works = 0;
    VkSurfaceCapabilitiesKHR caps;
    VkResult r = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pdev, srf, &caps);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkGetPhysicalDeviceSurfaceCapabilitiesKHR: ");
        vulk_err(stderr, r);
        return 1;
    }

    srfinfo->nimages = clamp_1D(caps.minImageCount, 3,
            caps.maxImageCount ? caps.maxImageCount : UINT32_MAX);
    srfinfo->extent = clamp_2D(caps.minImageExtent, caps.currentExtent, caps.maxImageExtent);
    srfinfo->xform = caps.currentTransform;

    *works = 1;
    return 0;
}

int check_srf_fmts(VkPhysicalDevice pdev, VkSurfaceKHR srf,
        Surface_info *srfinfo, bool *works) {
    int ret = 1;

    uint32_t nfmts;
    VkResult r = vkGetPhysicalDeviceSurfaceFormatsKHR(pdev, srf, &nfmts, NULL);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkGetPhysicalDeviceSurfaceFormatsKHR: ");
        vulk_err(stderr, r);
        goto out_retn;
    }
    VkSurfaceFormatKHR *fmts = malloc(nfmts * sizeof (VkSurfaceFormatKHR));
    if (!fmts) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto out_retn;
    }
    r = vkGetPhysicalDeviceSurfaceFormatsKHR(pdev, srf, &nfmts, fmts);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkGetPhysicalDeviceSurfaceFormatsKHR: ");
        vulk_err(stderr, r);
        goto out_free;
    }

    ret = 0;
    *works = 1;

    VkSurfaceFormatKHR preferred = {
        .format = VK_FORMAT_B8G8R8A8_SRGB,
        .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
    };
    uint32_t best = 0;
    for (uint32_t i = 0; i < nfmts; i++)
        if (fmts[i].colorSpace == preferred.colorSpace) {
            best = i;
            if (fmts[i].format == preferred.format) break;
        }
    srfinfo->fmt = fmts[best];

out_free:
    free(fmts);
out_retn:
    return ret;
}

int check_srf_prmodes(VkPhysicalDevice pdev, VkSurfaceKHR srf,
        Surface_info *srfinfo, bool *works) {
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
            srfinfo->prmode = modes[i];
            *works = 1;
            goto out_free;
        }

    *works = 0;

out_free:
    free(modes);
out_retn:
    return ret;
}

uint32_t clamp_1D(uint32_t low, uint32_t x, uint32_t high) {
    if (x < low) return low;
    if (x > high) return high;
    return x;
}

VkExtent2D clamp_2D(VkExtent2D low, VkExtent2D x, VkExtent2D high) {
    VkExtent2D ret = {
        .width = clamp_1D(low.width, x.width, high.width),
        .height = clamp_1D(low.height, x.height, high.height),
    };
    return ret;
}
