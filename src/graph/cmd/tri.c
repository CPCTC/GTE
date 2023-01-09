#include "graph/cmd/tri.h"
#include "graph/glfw.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int triangle_cmds_init(VkDevice dev, const Pools *pools, VkRenderPass rpass,
        const Frames *frames, VkPipeline pipe, VkExtent2D ext,
        Triangle_cmds *cmds) {
    cmds->nbufs = frames->nframes;
    cmds->bufs = malloc(cmds->nbufs * sizeof (VkCommandBuffer));
    if (!cmds->bufs) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto err_retn;
    }

    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,
        .commandPool = pools->pools[pools->by_q[GRAPHICS_Q]],
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = cmds->nbufs,
    };
    VkResult r = vkAllocateCommandBuffers(dev, &alloc_info, cmds->bufs);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkAllocateCommandBuffers: ");
        vulk_err(stderr, r);
        goto err_free_array;
    }

    for (uint32_t frame = 0; frame < frames->nframes; frame++) {
        VkCommandBufferBeginInfo begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = NULL,
            .flags = 0,
            .pInheritanceInfo = NULL,
        };
        r = vkBeginCommandBuffer(cmds->bufs[frame], &begin_info);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkBeginCommandBuffer: ");
            vulk_err(stderr, r);
            goto err_free_cmds;
        }

        VkClearValue clear = {.color = {.float32 = {0.0f, 0.0f, 0.0f, 1.0f}}};
        VkRenderPassBeginInfo rpass_info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = NULL,
            .renderPass = rpass,
            .framebuffer = frames->frames[frame],
            .renderArea = {
                .offset = {0, 0},
                .extent = ext,
            },
            .clearValueCount = 1,
            .pClearValues = &clear,
        };
        vkCmdBeginRenderPass(cmds->bufs[frame], &rpass_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(cmds->bufs[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipe);
        vkCmdDraw(cmds->bufs[frame], 3, 1, 0, 0);

        vkCmdEndRenderPass(cmds->bufs[frame]);

        r = vkEndCommandBuffer(cmds->bufs[frame]);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkEndCommandBuffer: ");
            vulk_err(stderr, r);
            goto err_free_cmds;
        }
    }

    return 0;

err_free_cmds:
    vkFreeCommandBuffers(dev, pools->pools[pools->by_q[GRAPHICS_Q]],
            cmds->nbufs, cmds->bufs);
err_free_array:
    free(cmds->bufs);
err_retn:
    cmds->nbufs = 0;
    cmds->bufs = NULL;
    return 1;
}

void triangle_cmds_destroy(VkDevice dev, const Pools *pools, Triangle_cmds *cmds) {
    vkFreeCommandBuffers(dev, pools->pools[pools->by_q[GRAPHICS_Q]],
            cmds->nbufs, cmds->bufs);
    free(cmds->bufs);
    cmds->nbufs = 0;
    cmds->bufs = NULL;
}
