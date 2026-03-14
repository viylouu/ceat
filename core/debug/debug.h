#pragma once

#include "../eaw/input.h"
typedef struct ear_font ear_font;

typedef struct eat_debug_desc{
    bool enabled;
    eaw_key key;
} eat_debug_desc;

extern eat_debug_desc debug;
extern ear_font* debug_font;

extern bool eat_debug_toggled;

typedef struct eat_debug_ll_obj eat_debug_ll_obj;

extern eat_debug_ll_obj* eat_debug_ll_first;
extern eat_debug_ll_obj* eat_debug_ll_last;


void
eat_debug_init(
    void
    );

void
eat_debug_stop(
    void
    );

void
eat_debug_try_do(
    void
    );


eat_debug_ll_obj*
eat_debug_add_obj(
    void* data,
    char* name,
    void (*debug_window)(void* obj, float x, float y, float w, float h)
    );

void
eat_debug_remove_obj(
    eat_debug_ll_obj* obj
    );

struct eat_debug_ll_obj{
    void* data;
    char* type_name;
    void (*debug_window)(void* obj, float x, float y, float w, float h);

    eat_debug_ll_obj* next;
    eat_debug_ll_obj* prev;
};
