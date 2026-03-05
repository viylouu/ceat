#pragma once

#include "core/eaw/eaw.h"
#include "core/ear/ear.h"
#include "core/eau/eau.h"

#include <stdint.h>
#include <stdbool.h>


extern int32_t eat_width;
extern int32_t eat_height;

extern float eat_time;
extern float eat_delta;
    extern double eat_time64;
    extern double eat_delta64;


typedef struct eat_init_opts {
    bool vsync;
} eat_init_opts;

void 
eat_init(
    char* title, 
    int32_t width, int32_t height,
    eat_init_opts opts
    );

void
eat_stop(
    void
    );

bool
eat_frame(
    void
    );
