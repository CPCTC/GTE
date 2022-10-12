#define HASH_IMPLEMENTATION
#include "blockset.h"

bool hash_eq(void *a, void *b) {
	return a == b;
}

uint32_t hash_fn(void *obj, uint32_t maxhash) {
	return (uintptr_t) obj % maxhash;
}
