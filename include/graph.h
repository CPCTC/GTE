#pragma once

typedef void *GRAPH;

GRAPH graph_init(void);
int mainloop(GRAPH g);
void graph_destroy(GRAPH *g);
