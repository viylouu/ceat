#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "arena.h"

typedef struct eau_object eau_object;
typedef struct eau_object_desc eau_object_desc;

typedef struct eau_object_ll eau_object_ll;

extern eau_object_ll* eau_object_ll_first;
extern eau_object_ll* eau_object_ll_last;


eau_object*
eau_create_object(
    eau_object_desc desc,
    void* data,
    eau_arena* arena
    );

void
eau_delete_object(
    eau_object* object
    );

// tickrate:
// - measured in delta time
// - if its 0, objects update each frame instead of on a fixed framerate

void
eau_set_object_tickrates(
    float delta
    );

void
eau_set_object_tickrate(
    eau_object* object,
    float delta
    );

void
eau_reset_object_tickrate(
    eau_object* object
    );

void
eau_init_object(
    eau_object* object
    );

void
eau_stop_object(
    eau_object* object
    );

void
eau_draw_object(
    eau_object* object
    );

void
eau_try_tick_object(
    eau_object* object
    );

void
eau_init_objects(
    void
    );

void
eau_stop_objects(
    void
    );

void
eau_draw_objects(
    void
    );

void
eau_try_tick_objects(
    void
    );


struct eau_object_ll{
    eau_object* obj;
    eau_object_ll* next;
    eau_object_ll* prev;
};

struct eau_object_desc{
    float* pos3d[3]; double* pos3d64[3];
    float* rot3d[3]; double* rot3d64[3];

    float* pos2d[2]; double* pos2d64[2];
    float* rot2d;    double* rot2d64;

    void (*init)(eau_object* obj);
    void (*tick)(eau_object* obj);
    void (*draw)(eau_object* obj);
    void (*stop)(eau_object* obj);
};

struct eau_object{
    eau_object_desc desc;

    eau_object_ll ll;

    void* data;

    bool use_global_tickrate;
    float delta;
    double last_time;

    eau_destructor* dest;
};
