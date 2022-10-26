#include "graph/pdev/ext.h"
#include "graph/nameset.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//

int check_dev_exts(VkPhysicalDevice pdev, uint32_t nexts, const char * const *exts, bool *works) {
    int ret = 1;

    uint32_t nprops;
    VkResult r = vkEnumerateDeviceExtensionProperties(pdev, NULL, &nprops, NULL);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumerateDeviceExtensionProperties: ");
        vulk_err(stderr, r);
        goto out_retn;
    }
    VkExtensionProperties *props = malloc(nprops * sizeof (VkExtensionProperties));
    if (!props) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto out_retn;
    }
    r = vkEnumerateDeviceExtensionProperties(pdev, NULL, &nprops, props);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumerateDeviceExtensionProperties: ");
        vulk_err(stderr, r);
        goto out_free_props;
    }

    NAMESET set = nameset_init(nprops, 0.0f);
    if (!set) {
        fprintf(stderr, "Can't nameset_init.\n");
        goto out_free_props;
    }
    for (uint32_t i = 0; i < nprops; i++) {
        if (nameset_set(set, props[i].extensionName, 0)) {
            fprintf(stderr, "Can't nameset_set.\n");
            goto out_free_nameset;
        }
    }

    VkPhysicalDeviceProperties pdevprop;
    vkGetPhysicalDeviceProperties(pdev, &pdevprop);

    ret = 0;
    *works = 1;
    for (uint32_t i = 0; i < nexts; i++)
        if (nameset_get(set, exts[i], NULL)) {
            fprintf(stderr, "Device %s: Unsupported extension %s\n", pdevprop.deviceName, exts[i]);
            *works = 0;
        }

out_free_nameset:
    nameset_free(&set);
out_free_props:
    free(props);
out_retn:
    return ret;
}
