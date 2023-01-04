#include "graph/pool.h"

int pools_init(VkDevice dev, const Queue_infos *q_infos, Pools *pools) {
    for (Queue_name q = 0; q < MAX_Q; q++)
        pools->used[q] = 0;

    for (Queue_name q = 0; q < MAX_Q; q++) {
        uint32_t fam = q_infos->fams[q];

        if (!pools->used[fam]) {
            pools->used[fam] = 1;
            VkCommandPoolCreateInfo info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .queueFamilyIndex = fam,
            };
            VkResult r = vkCreateCommandPool(dev, &info, NULL, pools->pools + fam);
            if (r != VK_SUCCESS) {
                fprintf(stderr, "Can't vkCreateCommandPool: ");
                vulk_err(stderr, r);
                while (q)
                    vkDestroyCommandPool(dev, pools->pools[q_infos->fams[--q]], NULL);
                return 1;
            }
        }
    }

    return 0;
}

void pools_destroy(VkDevice dev, Pools *pools) {
    for (uint32_t fam = 0; fam < MAX_Q; fam++)
        if (pools->used[fam]) {
            pools->used[fam] = 0;
            vkDestroyCommandPool(dev, pools->pools[fam], NULL);
            pools->pools[fam] = NULL;
        }
}
