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

int mainloop(GRAPH g) {
    (void) g;
    printf("mainloop\n");
    sleep(5);
    return 0;
}

void graph_destroy(GRAPH *hg) {
    Graph *g = *hg;
    win_close(&g->win);
    free(g);
    *hg = NULL;
}
