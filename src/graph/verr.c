#include "graph/verr.h"

#define cse(x) case x: err = #x; break;

void vulk_err_str(FILE *stream, VkResult r) {
    const char *err;
    switch (r) {
        cse(VK_SUCCESS);
        cse(VK_NOT_READY);
        cse(VK_TIMEOUT);
        cse(VK_EVENT_SET);
        cse(VK_EVENT_RESET);
        cse(VK_INCOMPLETE);
        cse(VK_ERROR_OUT_OF_HOST_MEMORY);
        cse(VK_ERROR_OUT_OF_DEVICE_MEMORY);
        cse(VK_ERROR_INITIALIZATION_FAILED);
        cse(VK_ERROR_DEVICE_LOST);
        cse(VK_ERROR_MEMORY_MAP_FAILED);
        cse(VK_ERROR_LAYER_NOT_PRESENT);
        cse(VK_ERROR_EXTENSION_NOT_PRESENT);
        cse(VK_ERROR_FEATURE_NOT_PRESENT);
        cse(VK_ERROR_INCOMPATIBLE_DRIVER);
        cse(VK_ERROR_TOO_MANY_OBJECTS);
        cse(VK_ERROR_FORMAT_NOT_SUPPORTED);
        cse(VK_ERROR_FRAGMENTED_POOL);
        cse(VK_ERROR_UNKNOWN);
        cse(VK_ERROR_OUT_OF_POOL_MEMORY);
        cse(VK_ERROR_INVALID_EXTERNAL_HANDLE);
        cse(VK_ERROR_FRAGMENTATION);
        cse(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
        cse(VK_PIPELINE_COMPILE_REQUIRED);
        default:
            fprintf(stream, "error %d\n", r);
            return;
    }
    fprintf(stream, "%s\n", err);
}
