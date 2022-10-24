#pragma once

#define HASH_UNAME NAMESET
#define HASH_LNAME nameset
#define HASH_KEY_TYPE const char *
#define HASH_VAL_TYPE char
#include "hash.h"

int nameset_to_array(NAMESET *n, uint32_t *size, const char ***arr);
