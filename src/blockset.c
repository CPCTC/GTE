#define HASH_IMPLEMENTATION
#include "blockset.h"

bool hash_eq(HASH_KEY_TYPE a, HASH_KEY_TYPE b) {
	return a == b;
}

uint32_t hash_fn(HASH_KEY_TYPE obj, uint32_t maxhash) {
	return (uintptr_t) obj % maxhash;
}
