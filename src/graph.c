#include "graph.h"
#include "graph/win.h"
#include <stdlib.h>

#include <unistd.h>
#include <stdio.h>

typedef struct {
    GLFWwindow *win;
} Graph;

GRAPH graph_init(void) {
    Graph *g = malloc(sizeof (Graph));
    if (!g) goto err_retn;
    g->win = win_open();
    if (!g->win) goto err_free;
    // ...
    return g;

err_free:
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
    win_close(&g->win);
    free(g);
    *hg = NULL;
}