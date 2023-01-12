#pragma once

typedef void *GRAPH;

GRAPH graph_init(void);
int graph_draw(GRAPH g);
void graph_destroy(GRAPH *g);
