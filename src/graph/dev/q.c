#include "graph/dev/q.h"
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//

static const float QUEUE_PRI = 1.0f;
static bool is_queue(Queue_name queue, VkQueueFamilyProperties family);

//

int create_queues(VkPhysicalDevice pdev, Queue_infos *q_infos) {
    uint32_t nfams;
    vkGetPhysicalDeviceQueueFamilyProperties(pdev, &nfams, NULL);
    VkQueueFamilyProperties *fams = malloc(nfams * sizeof (VkQueueFamilyProperties));
    if (!fams) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        return 1;
    }
    vkGetPhysicalDeviceQueueFamilyProperties(pdev, &nfams, fams);

    uint64_t create_fam_flags = 0;

    uint32_t nq_flags = 0;
    uint64_t q_flags = 0;

    for (uint32_t fam = 0; fam < nfams; fam++)
        for (Queue_name q = 0; q < MAX_Q; q++)
            if (!(q_flags & 1 << q))
                if (is_queue(q, fams[fam])) {
                    q_flags |= 1 << q;
                    q_infos->fams[q] = fam;
                    create_fam_flags |= 1 << fam;
                    if (++nq_flags == MAX_Q) goto end;
                }
    free(fams);
    return 1;
end:
    free(fams);

    uint32_t fam = 0;
    q_infos->ncreates = 0;
    while (create_fam_flags) {
        if (create_fam_flags & 1) {
            q_infos->creates[q_infos->ncreates++] = (VkDeviceQueueCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .queueFamilyIndex = fam,
                .queueCount = 1,
                .pQueuePriorities = &QUEUE_PRI,
            };
        }
        fam++;
        create_fam_flags >>= 1;
    }

    return 0;
}

bool is_queue(Queue_name queue, VkQueueFamilyProperties family) {
    switch (queue) {
        case (GRAPHICS_Q):
            return family.queueFlags & VK_QUEUE_GRAPHICS_BIT;
        case (MAX_Q):
            return 0;
    }

    // unreachable
    return *(bool *)NULL;
}
