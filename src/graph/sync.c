#include "graph/sync.h"

int sync_init(VkDevice dev, Sync *sync) {
    Sem_name sem;
    for (sem = 0; sem < MAX_SEM; sem++) {
        VkSemaphoreCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
        };
        VkResult r = vkCreateSemaphore(dev, &info, NULL, sync->sem + sem);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkCreateSemaphore: ");
            vulk_err(stderr, r);
            goto err_free_sems;
        }
    }

    return 0;

err_free_sems:
    while (sem) {
        vkDestroySemaphore(dev, sync->sem[--sem], NULL);
        sync->sem[sem] = NULL;
    }
    return 1;
}

void sync_destroy(VkDevice dev, Sync *sync) {
    for (Sem_name sem = 0; sem < MAX_SEM; sem++) {
        vkDestroySemaphore(dev, sync->sem[sem], NULL);
        sync->sem[sem] = NULL;
    }
}
