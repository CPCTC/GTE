#pragma once

#include <stdint.h>

int get_layers(uint32_t *size, const char ***layers);
int check_layers(uint32_t nlayers, const char * const *layers);
int get_exts(uint32_t *size, const char ***exts);
int check_exts(uint32_t nexts, const char * const *exts);
