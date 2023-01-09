#include "graph/pool.h"

int pools_init(VkDevice dev, const Queue_infos *q_infos, Pools *pools) {
    pools->npools = q_infos->ncreates;
    for (uint32_t pool = 0; pool < pools->npools; pool++) {
        VkCommandPoolCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .queueFamilyIndex = q_infos->creates[pool].queueFamilyIndex,
        };
        VkResult r = vkCreateCommandPool(dev, &info, NULL, pools->pools + pool);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkCreateCommandPool: ");
            vulk_err(stderr, r);
            while (pool)
                vkDestroyCommandPool(dev, pools->pools[--pool], NULL);
            return 1;
        }
    }

    for (Queue_name q = 0; q < MAX_Q; q++)
        pools->by_q[q] = q_infos->by_q[q];

    return 0;
}

void pools_destroy(VkDevice dev, Pools *pools) {
    for (uint32_t pool = 0; pool < pools->npools; pool++) {
        vkDestroyCommandPool(dev, pools->pools[pool], NULL);
        pools->pools[pool] = NULL;
    }
    pools->npools = 0;
}
