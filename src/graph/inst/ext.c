#include "graph/inst/ext.h"
#include "graph/inst/nameset.h"
#include "graph/glfw.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//

const char * const INST_EXTENSIONS[] = {
"VK_KHR_surface",
#ifdef DEBUG
"VK_EXT_debug_utils",
#endif
};

//

int get_layers(uint32_t *size, const char ***layers) {
    int ret = 1;

    NAMESET n = nameset_init_default();
    if (!n) {
        fprintf(stderr, "Can't create nameset.\n");
        goto out_retn;
    }
#ifdef DEBUG
    if (nameset_set(n, "VK_LAYER_KHRONOS_validation", 0)) {
        fprintf(stderr, "Can't update nameset\n");
        goto out_free_names;
    }
#endif
    if (nameset_to_array(n, size, layers)) {
        fprintf(stderr, "Can't make array of nameset.\n");
        goto out_free_names;
    }
    ret = 0;

out_free_names:
    nameset_free(&n);
out_retn:
    return ret;
}

int check_layers(uint32_t nlayers, const char * const *layers) {
    int ret = 1;

    uint32_t nprops;
    VkResult r = vkEnumerateInstanceLayerProperties(&nprops, NULL);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumerateInstanceLayerProperties: ");
        vulk_err_str(stderr, r);
        goto out_retn;
    }
    VkLayerProperties *props = malloc(nprops * sizeof (VkLayerProperties));
    if (!props) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto out_retn;
    }
    r = vkEnumerateInstanceLayerProperties(&nprops, props);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumerateInstanceLayerProperties: ");
        vulk_err_str(stderr, r);
        goto out_free_props;
    }

    NAMESET set = nameset_init(nprops, 0.0f);
    if (!set) {
        fprintf(stderr, "Can't nameset_init.\n");
        goto out_free_props;
    }
    for (uint32_t i = 0; i < nprops; i++) {
        if (nameset_set(set, props[i].layerName, 0)) {
            fprintf(stderr, "Can't nameset_set.\n");
            goto out_free_nameset;
        }
    }

    bool err = 0;
    for (uint32_t i = 0; i < nlayers; i++)
        if (nameset_get(set, layers[i], NULL)) {
            fprintf(stderr, "Unsupported Vulkan layer %s!\n", layers[i]);
            err = 1;
        }
    if (err) goto out_free_nameset;
    ret = 0;

out_free_nameset:
    nameset_free(&set);
out_free_props:
    free(props);
out_retn:
    return ret;
}

int get_exts(uint32_t *size, const char ***exts) {
    int ret = 1;

    NAMESET n = nameset_init_default();
    if (!n) {
        fprintf(stderr, "Can't create nameset.\n");
        goto out_retn;
    }

    uint32_t glfw_size;
    const char **glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_size);
    if (!glfw_exts) {
        fprintf(stderr, "Can't get GLFW required Vulkan extensions: %s\n", glfw_err_str());
        goto out_free_names;
    }
    for (uint32_t i = 0; i < glfw_size; i++)
        if (nameset_set(n, glfw_exts[i], 0))
            goto out_free_names;

    for (uint32_t i = 0; i < sizeof (INST_EXTENSIONS) / sizeof (INST_EXTENSIONS[0]); i++)
        if (nameset_set(n, INST_EXTENSIONS[i], 0))
            goto out_free_names;

    if (nameset_to_array(n, size, exts)) {
        fprintf(stderr, "Can't make array of nameset.\n");
        goto out_free_names;
    }
    ret = 0;

out_free_names:
    nameset_free(&n);
out_retn:
    return ret;
}

int check_exts(uint32_t nexts, const char * const *exts) {
    int ret = 1;

    uint32_t nprops;
    VkResult r = vkEnumerateInstanceExtensionProperties(NULL, &nprops, NULL);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumerateInstanceExtensionProperties: ");
        vulk_err_str(stderr, r);
        goto out_retn;
    }
    VkExtensionProperties *props = malloc(nprops * sizeof (VkExtensionProperties));
    if (!props) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto out_retn;
    }
    r = vkEnumerateInstanceExtensionProperties(NULL, &nprops, props);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkEnumerateInstanceExtensionProperties: ");
        vulk_err_str(stderr, r);
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

    bool err = 0;
    for (uint32_t i = 0; i < nexts; i++)
        if (nameset_get(set, exts[i], NULL)) {
            fprintf(stderr, "Unsupported Vulkan extension %s!\n", exts[i]);
            err = 1;
        }
    if (err) goto out_free_nameset;
    ret = 0;

out_free_nameset:
    nameset_free(&set);
out_free_props:
    free(props);
out_retn:
    return ret;
}
