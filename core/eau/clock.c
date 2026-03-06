#include "clock.h"
#include "../cutil.h"

#include "../eaw/eaw.h"

void
_eau_arena_clock_delete(
    void* clock
    ) { 
    eau_delete_clock(clock); 
}

eau_clock*
eau_create_clock(
    eau_arena* arena
    ) {
    eau_clock* clock = malloc(sizeof(eau_clock));
    *clock = (eau_clock){
        .time = 0,
        .time64 = 0,
        .delta = 0,
        .delta64 = 0,
        .speed = 1,
        };

    if (arena != NULL) eau_add_to_arena(arena, &clock->dest, clock, _eau_arena_clock_delete);
    return clock;
}

void
eau_delete_clock(
    eau_clock* clock
    ) {
    if (clock->dest != NULL) clock->dest->data = NULL;
    free(clock);
}

void
eau_reset_clock(
    eau_clock* clock
    ) {
    clock->time = 0;
}

void
eau_set_clock_speed(
    eau_clock* clock,
    float speed
    ) {
    clock->speed = speed;
}

void
eau_set_clock_time(
    eau_clock* clock,
    double time
    ) {
    clock->time = time;
    clock->time64 = time;
}

void
eau_update_clock(
    eau_clock* clock
    ) {
    clock->delta64 = eaw_delta * clock->speed;
    clock->delta = eaw_delta * clock->speed;
    clock->time64 += clock->delta64;
    clock->time = clock->time64;
}
