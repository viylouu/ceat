#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "arena.h"
#include "../debug/debug.h"

typedef struct eau_clock eau_clock;

typedef struct eau_clock_ll eau_clock_ll;

extern eau_clock_ll* eau_clock_ll_first;
extern eau_clock_ll* eau_clock_ll_last;


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
eau_start_clock(
    eau_clock* clock
    );

void
eau_stop_clock(
    eau_clock* clock
    );

void
eau_update_clocks(
    void
    );


struct eau_clock_ll{
    eau_clock* clock;
    eau_clock_ll* next;
    eau_clock_ll* prev;
};

struct eau_clock{
    eau_clock_ll ll;

    float time;
    float delta;
    double time64;
    double delta64;

    float speed;

    bool paused;

    eau_destructor* dest;
    eat_debug_ll_obj* deb_obj;
};
