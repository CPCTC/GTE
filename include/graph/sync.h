#pragma once

#include "graph/glfw.h"

typedef enum {
    PIPE_SEM,
    MAX_SEM,
} Sem_name;

typedef VkSemaphore Sem_grp[MAX_SEM]; // indexed by Sem_name

typedef struct {
    uint32_t nsync;
    Sem_grp *grps; // indexed by img
    VkFence *obj_f; // indexed by obj
    VkSemaphore *obj_s; // indexed by obj
} Sync;

int sync_init(VkDevice dev, uint32_t nsync, Sync *sync);
int sync_request(VkDevice dev, const Sync *sync, uint32_t *obj);
void sync_destroy(VkDevice dev, Sync *sync);
