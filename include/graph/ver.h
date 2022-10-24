#pragma once

#include <stdint.h>
#include <stdio.h>

extern const uint32_t TARGET_VULK_VER;
int check_version(uint32_t ver);
void version_warning(FILE *stream, uint32_t ver);
