#include "opts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//

static void usage(FILE *stream, const char *me);

//

int parse_opts(struct options *opt, int argc, const char *const *argv) {
    opt->file = NULL;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h")) {
            usage(stdout, argv[0]);
            exit(0);
        }
        else if (!opt->file) {
            opt->file = argv[i];
        }
        else {
            usage(stderr, argv[0]);
            return 2;
        }
    }
    return 0;
}

void usage(FILE *stream, const char *me) {
    fprintf(stream,
            "Usage: %s [OPTION]... [FILE]\n"
            " -h: help\n",
            me);
}
