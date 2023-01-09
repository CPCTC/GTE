#include "graph/frame.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int frames_init(VkDevice dev,
        const Images *imgs, VkRenderPass rpass, VkExtent2D ext,
        Frames *frames) {
    frames->nframes = imgs->nimgs;
    frames->frames = malloc(frames->nframes * sizeof (VkFramebuffer));
    if (!frames->frames) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto err_retn;
    }

    for (uint32_t i = 0; i < frames->nframes; i++) {
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

        VkResult r = vkCreateFramebuffer(dev, &info, NULL, frames->frames + i);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkCreateFramebuffer: ");
            vulk_err(stderr, r);
            while (i)
                vkDestroyFramebuffer(dev, frames->frames[i], NULL);
            goto err_free;
        }
    }

    return 0;

err_free:
    free(frames->frames);
err_retn:
    frames->nframes = 0;
    frames->frames = NULL;
    return 1;
}

void frames_destroy(VkDevice dev, Frames *frames) {
    for (uint32_t i = 0; i < frames->nframes; i++)
        vkDestroyFramebuffer(dev, frames->frames[i], NULL);
    free(frames->frames);
    frames->nframes = 0;
    frames->frames = NULL;
}
