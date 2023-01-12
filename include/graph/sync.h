#pragma once

#include "graph/glfw.h"

typedef enum {
    IMG_SEM,
    PIPE_SEM,
    MAX_SEM,
} Sem_name;

typedef struct {
    VkSemaphore sem[MAX_SEM]; // indexed by Sem_name
} Sync;

int sync_init(VkDevice dev, Sync *sync);
void sync_destroy(VkDevice dev, Sync *sync);
