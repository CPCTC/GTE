#include "buff.h"
#include "opts.h"
#include <stdlib.h>

//

int main(int argc, const char *const *argv) {
    struct options opt;
    if (parse_opts(&opt, argc, argv))
        return 2;

    struct buff_node *start;
    blockset blocks;
    if (open_buff(&start, &blocks, opt))
        return 1;

    // < Graphics stuff >
    // < load user config (shared library) >
    // < register user function as window input callback >
    // < begin mainloop >

//printf("mainloop here\n");
    while (start != NULL) {
        fputs(start->data, stdout);
        start = start->next;
    }
}
