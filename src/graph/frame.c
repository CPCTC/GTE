#include "graph/frame.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int frames_init(VkDevice dev,
        const Images *imgs, VkRenderPass rpass, VkExtent2D ext,
        VkFramebuffer **frames) {
    *frames = calloc(imgs->nimgs, sizeof (VkFramebuffer));
    if (!*frames) {
        fprintf(stderr, "Can't calloc: %s\n", strerror(errno));
        goto err_retn;
    }

    for (uint32_t i = 0; i < imgs->nimgs; i++) {
        VkFramebufferCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .renderPass = rpass,
            .attachmentCount = 1, // matching rpass
            .pAttachments = imgs->views + i,
            .width = ext.width,
            .height = ext.height,
            .layers = 1,
        };

        VkResult r = vkCreateFramebuffer(dev, &info, NULL, (*frames) + i);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkCreateFramebuffer: ");
            vulk_err(stderr, r);
            goto err_free_frames;
        }
    }

    return 0;

err_free_frames:
    for (uint32_t i = 0; i < imgs->nimgs; i++)
        if ((*frames)[i])
            vkDestroyFramebuffer(dev, (*frames)[i], NULL);
    free(*frames);
err_retn:
    *frames = NULL;
    return 1;
}

void frames_destroy(VkDevice dev, const Images *imgs, VkFramebuffer **frames) {
    for (uint32_t i = 0; i < imgs->nimgs; i++)
        vkDestroyFramebuffer(dev, (*frames)[i], NULL);
    free(*frames);
    *frames = NULL;
}
