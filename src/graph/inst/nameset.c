#define HASH_IMPLEMENTATION
#include "graph/inst/nameset.h"
#include <string.h>
#include <xxhash.h>

bool hash_eq(HASH_KEY_TYPE a, HASH_KEY_TYPE b) {
    return !strcmp(a, b);
}

uint32_t hash_fn(HASH_KEY_TYPE obj, uint32_t maxhash) {
    return XXH32(obj, strlen(obj), 0) % maxhash;
}

int nameset_to_array(NAMESET *n, uint32_t *size, const char ***arr) {
    *size = nameset_size(n);
    *arr = malloc(*size * sizeof (const char *));
    if (!*arr) goto err_retn;
    NAMESET_ITER it = nameset_begin(n);
    if (!it) goto err_free;
    uint32_t i = 0;
    while (nameset_next(it, (*arr) + i++, NULL));
    nameset_end(&it);
    return 0;

err_free:
    free(*arr);
err_retn:
    *size = 0;
    *arr = NULL;
    return 1;
}
