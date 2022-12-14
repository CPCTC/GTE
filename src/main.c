#include "buff.h"
#include "opts.h"
#include "graph.h"
#include <stdlib.h>

//

int main(int argc, const char *const *argv) {
    struct options opt;
    if (parse_opts(&opt, argc, argv))
        return 2;

    struct buff_node *start;
    BLOCKSET blocks;
    if (open_buff(&start, &blocks, opt))
        return 1;

    GRAPH graph = graph_init();
    if (!graph) {
        return 1;
    }

    // < load user config (shared library) >

    // < set mainloop callbacks >
    // < set mainloop data pointers >

    while (!graph_draw(graph));
    graph_destroy(&graph);
    close_buff(&blocks); start = NULL;
    return 0;
}
