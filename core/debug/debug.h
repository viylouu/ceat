#pragma once

#include "../eaw/input.h"
typedef struct ear_font ear_font;
typedef struct eau_arena eau_arena;

typedef struct eat_debug_desc{
    bool enabled;
    eaw_key key;
} eat_debug_desc;

typedef struct eat_debug_theme{
    ear_font* font;

    float bg_col[4];
    float text_col[4];

    float but_col[4];
    float sel_but_col[4];
} eat_debug_theme;

extern eat_debug_theme debug_theme;

extern eat_debug_desc debug;

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
    void (*debug_window)(void* obj, float x, float y, float w, float h, eat_debug_theme theme, int32_t* sel),
    eau_arena* arena
    );

void
eat_debug_remove_obj(
    eat_debug_ll_obj* obj
    );

void
eat_debug_get_screen_size(
    int32_t* width,
    int32_t* height
    );

struct eat_debug_ll_obj{
    void* data;
    char* type_name;
    void (*debug_window)(void* obj, float x, float y, float w, float h, eat_debug_theme theme, int32_t* sel);
    eau_arena* arena;

    eat_debug_ll_obj* next;
    eat_debug_ll_obj* prev;
};
