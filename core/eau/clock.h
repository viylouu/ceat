#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "arena.h"

typedef struct eau_clock eau_clock;


eau_clock*
eau_create_clock(
    eau_arena* arena
    );

void
eau_delete_clock(
    eau_clock* clock
    );

void
eau_reset_clock(
    eau_clock* clock
    );

void
eau_set_clock_speed(
    eau_clock* clock,
    float speed
    );

void
eau_set_clock_time(
    eau_clock* clock,
    double time
    );

void
eau_update_clock(
    eau_clock* clock
    );


struct eau_clock{
    float time;
    float delta;
    double time64;
    double delta64;

    float speed;

    eau_destructor* dest;
};
