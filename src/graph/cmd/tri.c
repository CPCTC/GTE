#include "graph/cmd/tri.h"
#include "graph/glfw.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int triangle_cmds_init(VkDevice dev, const Pools *pools, VkRenderPass rpass,
        uint32_t nframes, VkFramebuffer *frames,
        VkPipeline pipe, VkExtent2D ext,
        VkCommandBuffer **cmds) {
    *cmds = malloc(nframes * sizeof (VkCommandBuffer));
    if (!*cmds) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto err_retn;
    }

    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,
        .commandPool = pools->pools[pools->by_q[GRAPHICS_Q]],
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = nframes,
    };
    VkResult r = vkAllocateCommandBuffers(dev, &alloc_info, *cmds);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkAllocateCommandBuffers: ");
        vulk_err(stderr, r);
        goto err_free_array;
    }

    for (uint32_t frame = 0; frame < nframes; frame++) {
        VkCommandBufferBeginInfo begin_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = NULL,
            .flags = 0,
            .pInheritanceInfo = NULL,
        };
        r = vkBeginCommandBuffer((*cmds)[frame], &begin_info);
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
            .framebuffer = frames[frame],
            .renderArea = {
                .offset = {0, 0},
                .extent = ext,
            },
            .clearValueCount = 1,
            .pClearValues = &clear,
        };
        vkCmdBeginRenderPass((*cmds)[frame], &rpass_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline((*cmds)[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipe);
        vkCmdDraw((*cmds)[frame], 3, 1, 0, 0);

        vkCmdEndRenderPass((*cmds)[frame]);

        r = vkEndCommandBuffer((*cmds)[frame]);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkEndCommandBuffer: ");
            vulk_err(stderr, r);
            goto err_free_cmds;
        }
    }

    return 0;

err_free_cmds:
    vkFreeCommandBuffers(dev, pools->pools[pools->by_q[GRAPHICS_Q]],
            nframes, *cmds);
err_free_array:
    free(*cmds);
    *cmds = NULL;
err_retn:
    return 1;
}

void triangle_cmds_destroy(VkDevice dev, const Pools *pools,
        uint32_t ncmds, VkCommandBuffer **cmds) {
    vkFreeCommandBuffers(dev, pools->pools[pools->by_q[GRAPHICS_Q]],
            ncmds, *cmds);
    free(*cmds);
    *cmds = NULL;
}
