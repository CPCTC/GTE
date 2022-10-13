#pragma once

#include "blockset.h"
#include "opts.h"
#include <stdio.h>

struct buff_node {
    struct buff_node *prev, *next;
    // To get the size of the node header, the build script will
    // initially expand BUFF_DATA_CAP to the empty string.
    char data[BUFF_DATA_CAP];
};

int open_buff(struct buff_node **buff, BLOCKSET *blocks, struct options opt);
