#include "graph.h"
#include "graph/priv.h"

int graph_draw(GRAPH hg) {
    Graph *g = hg;

    uint32_t img;
    VkResult r = vkAcquireNextImageKHR(g->dev, g->sch, UINT64_MAX,
            g->sync.sem[IMG_SEM], NULL, &img);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkAcquireNextImageKHR: ");
        vulk_err(stderr, r);
        return 1;
    }

    VkSemaphoreSubmitInfo wait_sem_img = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext = NULL,
        .semaphore = g->sync.sem[IMG_SEM],
        .value = 0,
        .stageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        .deviceIndex = 0,
    };
    VkCommandBufferSubmitInfo cmd = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .pNext = NULL,
        .commandBuffer = g->tri_cmds.bufs[img],
        .deviceMask = 0,
    };
    VkSemaphoreSubmitInfo sig_sem_pipe = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext = NULL,
        .semaphore = g->sync.sem[PIPE_SEM],
        .value = 0,
        .stageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        .deviceIndex = 0,
    };
    VkSubmitInfo2 submit_info = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext = NULL,
        .flags = 0,
        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos = &wait_sem_img,
        .commandBufferInfoCount = 1,
        .pCommandBufferInfos = &cmd,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos = &sig_sem_pipe,
    };
    r = vkQueueSubmit2(g->qs[GRAPHICS_Q], 1, &submit_info, NULL);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkQueueSubmit2: ");
        vulk_err(stderr, r);
        return 1;
    }

    VkPresentInfoKHR present_info = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = NULL,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &g->sync.sem[PIPE_SEM],
        .swapchainCount = 1,
        .pSwapchains = &g->sch,
        .pImageIndices = &img,
        .pResults = NULL,
    };

    r = vkQueuePresentKHR(g->qs[PRESENT_Q], &present_info);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkQueuePresentKHR: ");
        vulk_err(stderr, r);
        return 1;
    }

    return 0;
}
