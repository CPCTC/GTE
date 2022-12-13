#include "graph/pipe/common/pipe.h"

int pipe_init(VkDevice dev, VkGraphicsPipelineCreateInfo *info, VkPipeline *pipe) {
    VkResult r = vkCreateGraphicsPipelines(dev, NULL, 1, info, NULL, pipe);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't VkCreateGraphicsPipelines: ");
        vulk_err(stderr, r);
        return 1;
    }
    return 0;
}

void pipe_destroy(VkDevice dev, VkPipeline *pipe) {
    vkDestroyPipeline(dev, *pipe, NULL); *pipe = NULL;
}
