#include "graph/sch.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//

static int check_srf(VkPhysicalDevice pdev, VkSurfaceKHR srf, Surface_info *srfinfo);
static int check_srf_caps(VkPhysicalDevice pdev, VkSurfaceKHR srf, Surface_info *srfinfo);
static int check_srf_fmts(VkPhysicalDevice pdev, VkSurfaceKHR srf, Surface_info *srfinfo);
static int check_srf_prmodes(VkPhysicalDevice pdev, VkSurfaceKHR srf, Surface_info *srfinfo);
static uint32_t clamp_1D(uint32_t low, uint32_t x, uint32_t high);
static VkExtent2D clamp_2D(VkExtent2D low, VkExtent2D x, VkExtent2D high);

//

Sch_status sch_init(VkDevice dev, VkPhysicalDevice pdev, VkSurfaceKHR srf,
        VkSwapchainKHR *sch, Surface_info *srfinfo) {
    if (check_srf(pdev, srf, srfinfo))
        return SCH_FAIL;
    if (srfinfo->extent.width == 0 || srfinfo->extent.height == 0)
        return SCH_AGAIN;
    VkSwapchainCreateInfoKHR info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .surface = srf,
        .minImageCount = srfinfo->nimages,
        .imageFormat = srfinfo->fmt.format,
        .imageColorSpace = srfinfo->fmt.colorSpace,
        .imageExtent = srfinfo->extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = NULL,
        .preTransform = srfinfo->xform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = srfinfo->prmode,
        .clipped = VK_TRUE,
        .oldSwapchain = NULL,
    };

    VkResult r = vkCreateSwapchainKHR(dev, &info, NULL, sch);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkCreateSwapchainKHR: ");
        vulk_err(stderr, r);
        return SCH_FAIL;
    }
    return SCH_SUCCESS;
}

void sch_destroy(VkDevice dev, VkSwapchainKHR *sch) {
    vkDestroySwapchainKHR(dev, *sch, NULL);
    *sch = NULL;
}

int check_srf(VkPhysicalDevice pdev, VkSurfaceKHR srf, Surface_info *srfinfo) {
    return
        check_srf_caps(pdev, srf, srfinfo) ||
        check_srf_fmts(pdev, srf, srfinfo) ||
        check_srf_prmodes(pdev, srf, srfinfo);
}

int check_srf_caps(VkPhysicalDevice pdev, VkSurfaceKHR srf, Surface_info *srfinfo) {
    VkSurfaceCapabilitiesKHR caps;
    VkResult r = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pdev, srf, &caps);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkGetPhysicalDeviceSurfaceCapabilitiesKHR: ");
        vulk_err(stderr, r);
        return 1;
    }

    srfinfo->nimages = clamp_1D(caps.minImageCount, 3, caps.maxImageCount ? caps.maxImageCount : (uint32_t) -1);
    srfinfo->extent = clamp_2D(caps.minImageExtent, caps.currentExtent, caps.maxImageExtent);
    srfinfo->xform = caps.currentTransform;

    return 0;
}

int check_srf_fmts(VkPhysicalDevice pdev, VkSurfaceKHR srf, Surface_info *srfinfo) {
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

int check_srf_prmodes(VkPhysicalDevice pdev, VkSurfaceKHR srf, Surface_info *srfinfo) {
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

    srfinfo->prmode = VK_PRESENT_MODE_FIFO_KHR; // Always supported
    for (uint32_t i = 0; i < nmodes; i++)
        if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            srfinfo->prmode = modes[i];
            break;
        }

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
