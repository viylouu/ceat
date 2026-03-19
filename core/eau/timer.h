#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "arena.h"
#include "../debug/debug.h"

typedef struct eau_timer eau_timer;

typedef struct eau_timer_ll eau_timer_ll;

extern eau_timer_ll* eau_timer_ll_first;
extern eau_timer_ll* eau_timer_ll_last;


eau_timer*
eau_create_timer(
    double wait,
    bool fixed,
    eau_arena* arena
    );

void
eau_delete_timer(
    eau_timer* timer
    );

void
eau_set_timer_onzero(
    eau_timer* timer,
    void (*onzero)(void* data),
    void* data
    );

void
eau_reset_timer(
    eau_timer* timer
    );

void
eau_set_timer_speed(
    eau_timer* timer,
    float speed
    );

void
eau_set_timer_time(
    eau_timer* timer,
    double time
    );

void
eau_set_timer_wait(
    eau_timer* timer,
    double wait
    );

void
eau_start_timer(
    eau_timer* timer
    );

void
eau_stop_timer(
    eau_timer* timer
    );

void
eau_update_timers(
    void
    );


struct eau_timer_ll{
    eau_timer* timer;
    eau_timer_ll* next;
    eau_timer_ll* prev;
};

struct eau_timer{
    eau_timer_ll ll;

    void (*on_zero)(void*);
    void* userdata;
    bool called;

    double wait;
    float time;
    float delta;
    double time64;
    double delta64;

    bool is_fixed;

    float speed;

    bool paused;

    eau_destructor* dest;
    eat_debug_ll_obj* deb_obj;
};
