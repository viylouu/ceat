#include "clock.h"
#include "../cutil.h"

#include "../eaw/eaw.h"
#include "core/eau/object.h"

eau_clock_ll* eau_clock_ll_first;
eau_clock_ll* eau_clock_ll_last;


void
_eau_arena_clock_delete(
    void* clock
    ) { 
    eau_delete_clock(clock); 
}

void
_eau_debug_clock_window(
    void* clock, 
    float x, float y,  float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

eau_clock*
eau_create_clock(
    bool fixed,
    eau_arena* arena
    ) {
    eau_clock* clock = malloc(sizeof(eau_clock));
    *clock = (eau_clock){
        .ll = (eau_clock_ll){
            .clock = clock,
            .prev = eau_clock_ll_last,
            .next = NULL,
            },

        .time = 0,
        .time64 = 0,
        .delta = 0,
        .delta64 = 0,
        .speed = 1,

        .is_fixed = fixed,

        .paused = true,

        .deb_obj = eat_debug_add_obj(
            clock,
            "clock",
            _eau_debug_clock_window,
            arena
            ),
        };

    if (eau_clock_ll_last != NULL) eau_clock_ll_last->next = &clock->ll;
    eau_clock_ll_last = &clock->ll;
    if (eau_clock_ll_first == NULL) eau_clock_ll_first = &clock->ll;

    if (arena != NULL) eau_add_to_arena(arena, &clock->dest, clock, _eau_arena_clock_delete);
    return clock;
}

void
eau_delete_clock(
    eau_clock* clock
    ) {
    if (clock->ll.prev != NULL) clock->ll.prev->next = clock->ll.next;
    else eau_clock_ll_first = clock->ll.next;

    if (clock->ll.next != NULL) clock->ll.next->prev = clock->ll.prev;
    else eau_clock_ll_last = clock->ll.prev;

    if (clock->dest != NULL) clock->dest->data = NULL;
    free(clock);
}

void
eau_reset_clock(
    eau_clock* clock
    ) {
    clock->time = 0;
    clock->time64 = 0;
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
eau_start_clock(
    eau_clock* clock
    ) {
    clock->paused = false;
}

void
eau_stop_clock(
    eau_clock* clock
    ) {
    clock->paused = true;
}

void
eau_update_clocks(
    void
    ) {
    if (eau_clock_ll_first == NULL) return;

    for (eau_clock_ll* item = eau_clock_ll_first; item != NULL; item = item->next) {
        if (item->clock->paused) continue;

        if (!item->clock->is_fixed) item->clock->delta64 = eaw_delta * item->clock->speed;
        else {
            if (!eau_tick_this_frame) continue;
            item->clock->delta64 = eau_tickrate * eau_tickspeed * item->clock->speed;
        }

        item->clock->time64 += item->clock->delta64;
        item->clock->delta = item->clock->delta64;
        item->clock->time = item->clock->time64;
    }
}


void
_eau_debug_clock_window(
    void* clock, 
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
    printf("hi :3");
}
