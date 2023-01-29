#include "graph/sch.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//

Sch_status sch_init(VkDevice dev, VkSurfaceKHR srf, const Surface_info *srfinfo,
        VkSwapchainKHR *sch) {
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
