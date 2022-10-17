#include "graph/debug.h"
#include <stdio.h>

//

static VkBool32 msg_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT *data,
        void *null);
static const char *sev_str(VkDebugUtilsMessageSeverityFlagBitsEXT s);
static const char *type_str(VkDebugUtilsMessageTypeFlagsEXT t);

//

VkDebugUtilsMessengerEXT debug_start(VkInstance inst) {
    VkDebugUtilsMessengerCreateInfoEXT info = debug_make_msgr_info(NULL);
    VkDebugUtilsMessengerEXT ret;
    VkResult r = fpi(inst,vkCreateDebugUtilsMessengerEXT)(inst, &info, NULL, &ret);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't vkCreateDebugUtilsMessengerEXT: ");
        vulk_err_str(stderr, r);
        return NULL;
    }
    return ret;
}

void debug_stop(VkInstance inst, VkDebugUtilsMessengerEXT *msgr) {
    fpi(inst,vkDestroyDebugUtilsMessengerEXT)(inst, *msgr, NULL); *msgr = NULL;
}

VkDebugUtilsMessengerCreateInfoEXT debug_make_msgr_info(void *pNext) {
    VkDebugUtilsMessengerCreateInfoEXT ret = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = pNext,
        .flags = 0,
        .messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = msg_callback,
        .pUserData = NULL,
    };
    return ret;
}

VkBool32 msg_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT sev,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT *data,
        void *null) {
    (void) null;
    fprintf(stderr, "%s %s: ", type_str(type), sev_str(sev));
    fprintf(stderr, "ID %d (%s): ", data->messageIdNumber, data->pMessageIdName);
    fprintf(stderr, "\t%s\n", data->pMessage);
    return VK_FALSE;
}

// Lowercase Noun
const char *sev_str(VkDebugUtilsMessageSeverityFlagBitsEXT s) {
    switch (s) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            return "info (verbose)";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            return "info";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            return "warning";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            return "error";
        default:
            return "message of unknown severity";
    }
}

// Capitalized Adjective
const char *type_str(VkDebugUtilsMessageTypeFlagsEXT t) {
    switch (t) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
            return "General";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            return "Validation";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            return "Performance";
        default:
            return "Unknown";
    }
}
