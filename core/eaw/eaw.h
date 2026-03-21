#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "window.h"
#include "input.h"

extern double eaw_time;
extern double eaw_delta;

void
eaw_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    );

void 
eaw_exit(
    void
    );

void
eaw_frame(
    void
    );

bool
eaw_is_open(
    void
    );
