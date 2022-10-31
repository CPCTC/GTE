#include "graph/img.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//

int img_init(VkDevice dev, VkSwapchainKHR sch, Surface_info srf_info, Images *imgs) {
    VkResult r = vkGetSwapchainImagesKHR(dev, sch, &imgs->nimgs, NULL);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkGetSwapchainImagesKHR: ");
        vulk_err(stderr, r);
        goto err_retn;
    }
    imgs->imgs = malloc(imgs->nimgs * sizeof (VkImage));
    if (!imgs->imgs) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto err_retn;
    }
    r = vkGetSwapchainImagesKHR(dev, sch, &imgs->nimgs, imgs->imgs);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkGetSwapchainImagesKHR: ");
        vulk_err(stderr, r);
        goto err_free_imgs;
    }

    imgs->views = calloc(imgs->nimgs, sizeof (VkImageView));
    if (!imgs->imgs) {
        fprintf(stderr, "Can't calloc: %s\n", strerror(errno));
        vulk_err(stderr, r);
        goto err_free_imgs;
    }

    for (uint32_t i = 0; i < imgs->nimgs; i++) {
        VkImageViewCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .image = imgs->imgs[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = srf_info.fmt.format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
        };

        r = vkCreateImageView(dev, &info, NULL, imgs->views + i);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkCreateImageView: ");
            vulk_err(stderr, r);
            goto err_free_views;
        }
    }

    return 0;

err_free_views:
    for (uint32_t i = 0; i < imgs->nimgs; i++)
        if (imgs->views[i])
            vkDestroyImageView(dev, imgs->views[i], NULL);
    free(imgs->views);
err_free_imgs:
    free(imgs->imgs);
err_retn:
    imgs->views = NULL;
    imgs->imgs = NULL;
    imgs->nimgs = 0;
    return 1;
}

void img_destroy(VkDevice dev, Images *imgs) {
    for (uint32_t i = 0; i < imgs->nimgs; i++)
        if (imgs->views[i])
            vkDestroyImageView(dev, imgs->views[i], NULL);
    free(imgs->views); imgs->views = NULL;
    free(imgs->imgs); imgs->imgs = NULL;
    imgs->nimgs = 0;
}
