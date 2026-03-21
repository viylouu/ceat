#pragma once

#include <stdint.h>

#define EAR_VK_MAX_FRAMES_IN_FLIGHT 3

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

void
ear_vk_frame(
    void
    );
