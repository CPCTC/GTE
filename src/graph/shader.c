#include "graph/shader.h"
#include "gen/shader.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int shaders_init(VkDevice dev, Shaders *shaders) {
    shaders->nshaders = SHADER_INFOS.nshaders;
    shaders->mods = NULL;
    shaders->stages = NULL;
    if (!SHADER_INFOS.nshaders) return 0;

    shaders->mods = malloc(SHADER_INFOS.nshaders * sizeof (VkShaderModule));
    if (!shaders->mods) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto err_retn;
    }

    shaders->stages = malloc(SHADER_INFOS.nshaders * sizeof (VkShaderStageFlagBits));
    if (!shaders->stages) {
        fprintf(stderr, "Can't malloc: %s\n", strerror(errno));
        goto err_free_mods;
    }

    for (uint32_t i = 0; i < SHADER_INFOS.nshaders; i++) {
        VkShaderModuleCreateInfo create = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .codeSize = SHADER_INFOS.shaders[i].end - SHADER_INFOS.shaders[i].start,
            .pCode = (const uint32_t *) SHADER_INFOS.shaders[i].start,
        };
        VkResult r = vkCreateShaderModule(dev, &create, NULL, shaders->mods + i);
        if (r != VK_SUCCESS) {
            fprintf(stderr, "Can't vkCreateShaderModule: ");
            vulk_err(stderr, r);
            while (i)
                vkDestroyShaderModule(dev, shaders->mods[--i], NULL);
            goto err_free_stages;
        }

        shaders->stages[i] = SHADER_INFOS.shaders[i].stage;
    }

    return 0;

err_free_stages:
    free(shaders->stages); shaders->stages = NULL;
err_free_mods:
    free(shaders->mods); shaders->mods = NULL;
err_retn:
    shaders->nshaders = 0;
    return 1;
}

void shaders_destroy(VkDevice dev, Shaders *shaders) {
    for (uint32_t i = 0; i < shaders->nshaders; i++)
        vkDestroyShaderModule(dev, shaders->mods[i], NULL);
    free(shaders->stages);
    free(shaders->mods);
    shaders->nshaders = 0;
}
