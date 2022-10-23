#include "graph/win.h"
#include <stdio.h>

static int default_win_size(int *w, int *h);

GLFWwindow *win_open(void) {
    if (glfwInit() != GLFW_TRUE) {
        fprintf(stderr, "Can't init GLFW: %s\n", glfw_err_str());
        goto err_retn;
    }

    int w, h;
    if (default_win_size(&w, &h)) goto err_terminate;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *ret = glfwCreateWindow(w, h, APPNAME, NULL, NULL);
    if (!ret) {
        fprintf(stderr, "Can't open window: %s\n", glfw_err_str());
        goto err_terminate;
    }

    return ret;

err_terminate:
    glfwTerminate();
err_retn:
    return NULL;
}

void win_close(GLFWwindow **w) {
    glfwDestroyWindow(*w); *w = NULL;
    glfwTerminate();
}

VkSurfaceKHR srf_init(VkInstance inst, GLFWwindow *win) {
    VkSurfaceKHR srf;
    VkResult r = glfwCreateWindowSurface(inst, win, NULL, &srf);
    if (r != VK_SUCCESS) {
        fprintf(stderr, "Can't glfwCreateWindowSurface: %s (", glfw_err_str());
        vulk_err_str(stderr, r);
        fprintf(stderr, ")\n");
        return NULL;
    }
    return srf;
}

void srf_destroy(VkInstance inst, VkSurfaceKHR *srf) {
    vkDestroySurfaceKHR(inst, *srf, NULL);
    *srf = NULL;
}

int default_win_size(int *w, int *h) {
    GLFWmonitor *m = glfwGetPrimaryMonitor();
    if (!m) {
        fprintf(stderr, "Can't find a monitor.\n");
        return 1;
    }
    glfwGetMonitorWorkarea(m, NULL, NULL, w, h);
    const char *err;
    if (glfwGetError(&err) != GLFW_NO_ERROR) {
        fprintf(stderr, "Can't find monitor size: %s\n", err);
        return 1;
    }
    *w /= 2;
    *h /= 2;
    return 0;
}
