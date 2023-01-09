#include "graph/pdev/q.h"
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//

static const float QUEUE_PRI = 1.0f;
int is_queue(Queue_name queue, VkPhysicalDevice pdev,
        VkQueueFamilyProperties* fams, uint32_t fam,
        VkSurfaceKHR srf,
        bool *ret);

//

int create_queues(VkPhysicalDevice pdev, VkSurfaceKHR srf, Queue_infos *q_infos, bool *works) {
    uint32_t nfams;
    vkGetPhysicalDeviceQueueFamilyProperties(pdev, &nfams, NULL);
    VkQueueFamilyProperties *fams = malloc(nfams * sizeof (VkQueueFamilyProperties));
    if (!fams) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        return 1;
    }
    vkGetPhysicalDeviceQueueFamilyProperties(pdev, &nfams, fams);

    uint64_t have_q = 0;
    uint64_t all_queues = 0;
    for (Queue_name q = 0; q < MAX_Q; q++)
        all_queues |= 1 << q;

    q_infos->ncreates = 0;

    for (uint32_t fam = 0; fam < nfams; fam++) {
        bool create_fam = 0;

        for (Queue_name q = 0; q < MAX_Q; q++)
            if (!(have_q & 1 << q)) {
                bool is_q;
                if (is_queue(q, pdev, fams, fam, srf, &is_q)) {
                    free(fams);
                    return 1;
                }
                if (is_q) {
                    create_fam = 1;
                    q_infos->by_q[q] = q_infos->ncreates;
                    have_q |= 1 << q;
                }
            }

        if (create_fam) {
            q_infos->creates[q_infos->ncreates++] = (VkDeviceQueueCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .queueFamilyIndex = fam,
                .queueCount = 1,
                .pQueuePriorities = &QUEUE_PRI,
            };
        }
    }

    free(fams);

    if (have_q != all_queues) {
        VkPhysicalDeviceProperties prop;
        vkGetPhysicalDeviceProperties(pdev, &prop);
        fprintf(stderr, "Device %s: Not enough queues.\n", prop.deviceName);
        *works = 0;
        return 0;
    }

    *works = 1;
    return 0;
}

int is_queue(Queue_name queue, VkPhysicalDevice pdev,
        VkQueueFamilyProperties* fams, uint32_t fam,
        VkSurfaceKHR srf,
        bool *ret) {
    switch (queue) {
        case (GRAPHICS_Q):
            *ret = fams[fam].queueFlags & VK_QUEUE_GRAPHICS_BIT;
            break;
        case (PRESENT_Q):
            {
                VkBool32 vkret;
                VkResult r = vkGetPhysicalDeviceSurfaceSupportKHR(pdev, fam, srf, &vkret);
                if (r != VK_SUCCESS) {
                    fprintf(stderr, "Can't vkGetPhysicalDeviceSurfaceSupportKHR: ");
                    vulk_err(stderr, r);
                    return 1;
                }
                *ret = vkret == VK_TRUE;
            }
            break;
        case (MAX_Q):
            *ret = 0;
            break;
    }
    return 0;
}
