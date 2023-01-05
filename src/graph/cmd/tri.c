#include "graph/cmd/tri.h"
#include <stdio.h>

int draw_cmds_init(VkDevice dev,
        const Queue_infos *q_infos, const Pools *pools,
        VkCommandBuffer *draw);

void draw_cmds_destroy(VkDevice dev,
        const Queue_infos *q_infos, const Pools *pools,
        VkCommandBuffer *draw);

//

int triangle_cmds_init(VkDevice dev,
        const Queue_infos *q_infos, const Pools *pools,
        Triangle_cmds *tri_cmds) {
    if (draw_cmds_init(dev, q_infos, pools, &tri_cmds->draw))
        goto err_retn;

    //present_cmds_init(...);

    return 0;

err_retn:
    return 1;
}

void triangle_cmds_destroy(VkDevice dev,
        const Queue_infos *q_infos, const Pools *pools,
        Triangle_cmds *tri_cmds) {
    //present_cmds_destroy(...);
    draw_cmds_destroy(dev, q_infos, pools, &tri_cmds->draw);
}

int draw_cmds_init(VkDevice dev,
        const Queue_infos *q_infos, const Pools *pools,
        VkCommandBuffer *cmds) {
    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,
        .commandPool = pools->pools[q_infos->fams[GRAPHICS_Q]],
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    VkResult r = vkAllocateCommandBuffers(dev, &alloc_info, cmds);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkAllocateCommandBuffers: ");
        vulk_err(stderr, r);
        goto err_retn;
    }

    VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,
        .flags = 0,
        .pInheritanceInfo = NULL,
    };
    r = vkBeginCommandBuffer(*cmds, &begin_info);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkBeginCommandBuffer: ");
        vulk_err(stderr, r);
        goto err_free;
    }

    // draw triangle here

    r = vkEndCommandBuffer(*cmds);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEndCommandBuffer: ");
        vulk_err(stderr, r);
        goto err_free;
    }

    return 0;

err_free:
    draw_cmds_destroy(dev, q_infos, pools, cmds);
err_retn:
    return 1;
}

void draw_cmds_destroy(VkDevice dev,
        const Queue_infos *q_infos, const Pools *pools,
        VkCommandBuffer *cmds) {
    vkFreeCommandBuffers(dev, pools->pools[q_infos->fams[GRAPHICS_Q]], 1, cmds);
    *cmds = NULL;
}
