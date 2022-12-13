#include "graph/pipe/common/shader.h"

void stage_infos_init(const Shaders *shaders,
        uint32_t nstages, const uint32_t *names,
        VkPipelineShaderStageCreateInfo *infos) {
    for (uint32_t i = 0; i < nstages; i++) {
        infos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        infos[i].pNext = NULL;
        infos[i].flags = 0;
        infos[i].stage = shaders->stages[names[i]];
        infos[i].module = shaders->mods[names[i]];
        infos[i].pName = "main";
        infos[i].pSpecializationInfo = NULL;
    }
}
