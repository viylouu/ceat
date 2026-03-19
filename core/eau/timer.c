#include "timer.h"
#include "../cutil.h"

#include "../eaw/eaw.h"
#include "core/eau/object.h"

eau_timer_ll* eau_timer_ll_first;
eau_timer_ll* eau_timer_ll_last;


void
_eau_arena_timer_delete(
    void* timer
    ) { 
    eau_delete_timer(timer); 
}

void
_eau_debug_timer_window(
    void* timer, 
    float x, float y,  float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

eau_timer*
eau_create_timer(
    double wait,
    bool fixed,
    eau_arena* arena
    ) {
    eau_timer* timer = malloc(sizeof(eau_timer));
    *timer = (eau_timer){
        .ll = (eau_timer_ll){
            .timer = timer,
            .prev = eau_timer_ll_last,
            .next = NULL,
            },

        .wait = wait,
        .time = wait,
        .time64 = wait,
        .delta = 0,
        .delta64 = 0,
        .speed = 1,
        .called = false,

        .is_fixed = fixed,

        .paused = true,

        .deb_obj = eat_debug_add_obj(
            timer,
            "timer",
            _eau_debug_timer_window,
            arena
            ),
        };

    if (eau_timer_ll_last != NULL) eau_timer_ll_last->next = &timer->ll;
    eau_timer_ll_last = &timer->ll;
    if (eau_timer_ll_first == NULL) eau_timer_ll_first = &timer->ll;

    if (arena != NULL) eau_add_to_arena(arena, &timer->dest, timer, _eau_arena_timer_delete);
    return timer;
}

void
eau_delete_timer(
    eau_timer* timer
    ) {
    if (timer->ll.prev != NULL) timer->ll.prev->next = timer->ll.next;
    else eau_timer_ll_first = timer->ll.next;

    if (timer->ll.next != NULL) timer->ll.next->prev = timer->ll.prev;
    else eau_timer_ll_last = timer->ll.prev;

    if (timer->dest != NULL) timer->dest->data = NULL;
    free(timer);
}

void
eau_set_timer_onzero(
    eau_timer* timer,
    void (*onzero)(void* data),
    void* data
    ) {
    timer->on_zero = onzero;
    timer->userdata = data;
    timer->called = false;
}

void
eau_reset_timer(
    eau_timer* timer
    ) {
    timer->time = timer->wait;
    timer->time64 = timer->wait;
    timer->called = false;
}

void
eau_set_timer_speed(
    eau_timer* timer,
    float speed
    ) {
    timer->speed = speed;
}

void
eau_set_timer_time(
    eau_timer* timer,
    double time
    ) {
    timer->time = time;
    timer->time64 = time;
}

void
eau_start_timer(
    eau_timer* timer
    ) {
    timer->paused = false;
}

void
eau_stop_timer(
    eau_timer* timer
    ) {
    timer->paused = true;
}

void
eau_update_timers(
    void
    ) {
    if (eau_timer_ll_first == NULL) return;

    for (eau_timer_ll* item = eau_timer_ll_first; item != NULL;) {
        eau_timer_ll* next = item->next;

        if (item->timer->paused) { item = next; continue; }

        if (!item->timer->is_fixed) item->timer->delta64 = eaw_delta * item->timer->speed;
        else item->timer->delta64 = eau_tickrate * eau_tickspeed * item->timer->speed;

        item->timer->time64 -= item->timer->delta64;
        item->timer->delta = item->timer->delta64;
        item->timer->time = item->timer->time64;

        if (item->timer->time64 < 0) {
            item->timer->time64 = 0;
            if (!item->timer->called) {
                item->timer->called = true;
                if (item->timer->on_zero) item->timer->on_zero(item->timer->userdata);
            }
        }

        item = next;
    }
}


void
_eau_debug_timer_window(
    void* timer, 
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
    printf("hi :3");
}

