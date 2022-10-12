#pragma once

struct options {
    const char *file;
};

int parse_opts(struct options *opt, int argc, const char *const *argv);
