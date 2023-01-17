#include "graph/sync.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int sync_init(VkDevice dev, uint32_t nsync, Sync *sync) {
    sync->nsync = nsync;

    sync->grps = malloc(sync->nsync * sizeof (Sem_grp));
    if (!sync->grps) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto err_retn;
    }

    sync->obj_f = malloc(sync->nsync * sizeof (VkFence));
    if (!sync->obj_f) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto err_free_grps;
    }

    sync->obj_s = malloc(sync->nsync * sizeof (VkSemaphore));
    if (!sync->obj_s) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto err_free_obj_f;
    }

    VkResult r;
    VkSemaphoreCreateInfo sem_info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
    };

    uint32_t grp;
    for (grp = 0; grp < sync->nsync; grp++)
        for (Sem_name sem = 0; sem < MAX_SEM; sem++) {
            r = vkCreateSemaphore(dev, &sem_info, NULL, &sync->grps[grp][sem]);
            if (r != VK_SUCCESS) {
                fprintf(stderr, "Can't vkCreateSemaphore: ");
                vulk_err(stderr, r);
                while (sem)
                    vkDestroySemaphore(dev, sync->grps[grp][--sem], NULL);
                goto err_free_grp_sems;
            }
        }

    uint32_t obj_f;
    for (obj_f = 0; obj_f < sync->nsync; obj_f++) {
        VkFenceCreateInfo fence_info = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = NULL,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT,
        };
        r = vkCreateFence(dev, &fence_info, NULL, &sync->obj_f[obj_f]);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkCreateFence: ");
            vulk_err(stderr, r);
            goto err_free_obj_f_fences;
        }
    }

    uint32_t obj_s;
    for (obj_s = 0; obj_s < sync->nsync; obj_s++) {
        r = vkCreateSemaphore(dev, &sem_info, NULL, &sync->obj_s[obj_s]);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkCreateSemaphore: ");
            vulk_err(stderr, r);
            goto err_free_obj_s_sems;
        }
    }

    return 0;

err_free_obj_s_sems:
    while (obj_s)
        vkDestroySemaphore(dev, sync->obj_s[--obj_s], NULL);
err_free_obj_f_fences:
    while (obj_f)
        vkDestroyFence(dev, sync->obj_f[--obj_f], NULL);
err_free_grp_sems:
    while (grp) {
        grp--;
        for (Sem_name sem = 0; sem < MAX_SEM; sem++)
            vkDestroySemaphore(dev, sync->grps[grp][sem], NULL);
    }
    free(sync->obj_s);
err_free_obj_f:
    free(sync->obj_f);
err_free_grps:
    free(sync->grps);
err_retn:
    sync->nsync = 0;
    sync->grps = NULL;
    sync->obj_f = NULL;
    sync->obj_s = NULL;
    return 1;
}

int sync_request(VkDevice dev, const Sync *sync, uint32_t *obj) {
    VkResult r;
    do
        r = vkWaitForFences(dev, sync->nsync, sync->obj_f, VK_FALSE, UINT64_MAX);
    while (r == VK_TIMEOUT);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkWaitForFences: ");
        vulk_err(stderr, r);
        return 1;
    }
    uint32_t i;
    for (i = 0; 1; i = (i + 1) % sync->nsync) {
        r = vkGetFenceStatus(dev, sync->obj_f[i]);
        switch (r) {
            case VK_SUCCESS:
                goto brk;
            case VK_NOT_READY:
                continue;
            default:
                fprintf(stderr, "Can't vkGetFenceStatus: ");
                vulk_err(stderr, r);
                return 1;
        }
    }
brk:

    r = vkResetFences(dev, 1, &sync->obj_f[i]);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkResetFences: ");
        vulk_err(stderr, r);
        return 1;
    }

    *obj = i;
    return 0;
}

void sync_destroy(VkDevice dev, Sync *sync) {
    for (uint32_t obj_s = 0; obj_s < sync->nsync; obj_s++)
        vkDestroySemaphore(dev, sync->obj_s[obj_s], NULL);

    for (uint32_t obj_f = 0; obj_f < sync->nsync; obj_f++)
        vkDestroyFence(dev, sync->obj_f[obj_f], NULL);

    for (uint32_t grp = 0; grp < sync->nsync; grp++)
        for (Sem_name sem = 0; sem < MAX_SEM; sem++)
            vkDestroySemaphore(dev, sync->grps[grp][sem], NULL);

    free(sync->obj_s);

    free(sync->obj_f);

    free(sync->grps);

    sync->nsync = 0;
    sync->grps = NULL;
    sync->obj_f = NULL;
    sync->obj_s = NULL;
}
