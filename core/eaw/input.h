#pragma once

#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum eaw_key_state{
    EAW_STATE_RELEASED,
    EAW_STATE_PRESSED,
    EAW_STATE_HELD,
    EAW_STATE_INACTIVE,
} eaw_key_state;

typedef enum eaw_key{
    EAW_KEY_ESCAPE,
    EAW_KEY_CAPS,
    EAW_KEY_SPACE, EAW_KEY_TAB,
    EAW_KEY_BACKSPACE, EAW_KEY_DELETE,
    EAW_KEY_HOME, EAW_KEY_END,
    EAW_KEY_PAGE_UP, EAW_KEY_PAGE_DOWN,
    EAW_KEY_INSERT,

    EAW_KEY_LSHIFT, EAW_KEY_RSHIFT,
    EAW_KEY_LCTRL, EAW_KEY_RCTRL,
    EAW_KEY_LALT, EAW_KEY_RALT,
    EAW_KEY_LSUPER, EAW_KEY_RSUPER,

    EAW_KEY_UP, EAW_KEY_LEFT, EAW_KEY_RIGHT, EAW_KEY_DOWN,

    EAW_KEY_A, EAW_KEY_B, EAW_KEY_C, EAW_KEY_D, EAW_KEY_E,
    EAW_KEY_F, EAW_KEY_G, EAW_KEY_H, EAW_KEY_I, EAW_KEY_J,
    EAW_KEY_K, EAW_KEY_L, EAW_KEY_M, EAW_KEY_N, EAW_KEY_O,
    EAW_KEY_P, EAW_KEY_Q, EAW_KEY_R, EAW_KEY_S, EAW_KEY_T,
    EAW_KEY_U, EAW_KEY_V, EAW_KEY_W, EAW_KEY_X, EAW_KEY_Y, EAW_KEY_Z,
    
    EAW_KEY_1, EAW_KEY_2, EAW_KEY_3, EAW_KEY_4, EAW_KEY_5,
    EAW_KEY_6, EAW_KEY_7, EAW_KEY_8, EAW_KEY_9, EAW_KEY_0,

    EAW_KEY_F1, EAW_KEY_F2, EAW_KEY_F3, EAW_KEY_F4,
    EAW_KEY_F5, EAW_KEY_F6, EAW_KEY_F7, EAW_KEY_F8,
    EAW_KEY_F9, EAW_KEY_F10, EAW_KEY_F11, EAW_KEY_F12,

    EAW_KEY_LAST,
} eaw_key;

typedef enum eaw_mouse{
    EAW_MOUSE_LEFT, EAW_MOUSE_RIGHT, EAW_MOUSE_MIDDLE,
    EAW_MOUSE_LAST,
} eaw_mouse;


extern float eaw_mouse_x;
extern float eaw_mouse_y;
    extern double eaw_mouse_x64;
    extern double eaw_mouse_y64;
extern float eaw_mouse_delta_x;
extern float eaw_mouse_delta_y;
    extern double eaw_mouse_delta_x64;
    extern double eaw_mouse_delta_y64;
extern float eaw_mouse_scroll_x;
extern float eaw_mouse_scroll_y;
    extern double eaw_mouse_scroll_x64;
    extern double eaw_mouse_scroll_y64;


bool
eaw_is_key(
    eaw_key key
    );
bool
eaw_is_key_pressed(
    eaw_key key
    );
bool
eaw_is_key_released(
    eaw_key key
    );

bool
eaw_is_mouse(
    eaw_mouse mouse
    );
bool
eaw_is_mouse_pressed(
    eaw_mouse mouse
    );
bool
eaw_is_mouse_released(
    eaw_mouse mouse
    );


typedef enum eaw_mouse_mode{
    EAW_MOUSE_MODE_NORMAL,
    EAW_MOUSE_MODE_HIDDEN,
    EAW_MOUSE_MODE_LOCKED,
} eaw_mouse_mode;

void
eaw_set_mouse_mode(
    eaw_mouse_mode mode
    );



void
eaw_input_init(
    void
    );

void
eaw_input_frame(
    void
    );
