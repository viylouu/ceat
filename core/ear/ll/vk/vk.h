#pragma once

#include <stdint.h>

void 
ear_vk_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    );
void
ear_vk_exit(
    void
    );
