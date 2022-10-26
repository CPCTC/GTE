#include "graph.h"
#include "graph/debug.h"
#include "graph/dev.h"
#include "graph/inst.h"
#include "graph/win.h"
#include <stdlib.h>

#include <unistd.h>
#include <stdio.h>

typedef struct {
    GLFWwindow *win;
    VkInstance inst;
    VkDebugUtilsMessengerEXT msgr;
    VkSurfaceKHR srf;
    VkDevice dev;
    Queues qs;
} Graph;

GRAPH graph_init(void) {
    Graph *g = malloc(sizeof (Graph));
    if (!g) goto err_retn;

    g->win = win_open();
    if (!g->win) goto err_free_g;

    g->inst = inst_create();
    if (!g->inst) goto err_close_win;

#ifdef DEBUG
    g->msgr = debug_start(g->inst);
    if (!g->msgr) goto err_free_inst;
#endif

    g->srf = srf_init(g->inst, g->win);
    if (!g->srf) goto err_stop_debug;

    g->dev = dev_init(g->inst, g->srf, g->qs);
    if (!g->dev) goto err_free_srf;

    // ...
    return g;

err_free_srf:
    srf_destroy(g->inst, &g->srf);
err_stop_debug:
#ifdef DEBUG
    debug_stop(g->inst, &g->msgr);
err_free_inst:
#endif
    inst_destroy(&g->inst);
err_close_win:
    win_close(&g->win);
err_free_g:
    free(g);
err_retn:
    return NULL;
}

int mainloop(GRAPH hg) {
    Graph *g = hg;
    (void) g;

    uint32_t ver;
    vkEnumerateInstanceVersion(&ver);
    printf("Welcome to Vulkan v.%u.%u.%u-var%u)!\n",
            VK_API_VERSION_MAJOR(ver),
            VK_API_VERSION_MINOR(ver),
            VK_API_VERSION_PATCH(ver),
            VK_API_VERSION_VARIANT(ver));
    sleep(5);
    return 0;
}

void graph_destroy(GRAPH *hg) {
    Graph *g = *hg;
    dev_destroy(&g->dev, g->qs);
    srf_destroy(g->inst, &g->srf);
#ifdef DEBUG
    debug_stop(g->inst, &g->msgr);
#endif
    inst_destroy(&g->inst);
    win_close(&g->win);
    free(g);
    *hg = NULL;
}
