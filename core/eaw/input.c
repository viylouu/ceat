#include "input.h"
//#include "../cutil.h"

#include "window.h"
#include "../eau/object.h"

#include <GLFW/glfw3.h>

float eaw_mouse_x;
float eaw_mouse_y;
    double eaw_mouse_x64;
    double eaw_mouse_y64;
float eaw_mouse_delta_x;
float eaw_mouse_delta_y;
    double eaw_mouse_delta_x64;
    double eaw_mouse_delta_y64;
float eaw_mouse_scroll_x;
float eaw_mouse_scroll_y;
    double eaw_mouse_scroll_x64;
    double eaw_mouse_scroll_y64;

char eaw_text_input[1024];
uint32_t eaw_text_input_chars;

char eaw_tick_text_input[1024];
uint32_t eaw_tick_text_input_chars;

double last_mouse_x;
double last_mouse_y;

eaw_key_state keys[EAW_KEY_LAST];
eaw_key_state mouses[EAW_MOUSE_LAST];

eaw_key_state tick_keys[EAW_KEY_LAST];
eaw_key_state tick_mouses[EAW_MOUSE_LAST];

bool
eaw_is_key(
    eaw_key key
    ) {
    return keys[key] == EAW_STATE_PRESSED || keys[key] == EAW_STATE_HELD;
}
bool
eaw_is_key_pressed(
    eaw_key key
    ) {
    return keys[key] == EAW_STATE_PRESSED;
}
bool
eaw_is_key_released(
    eaw_key key
    ) {
    return keys[key] == EAW_STATE_RELEASED;
}

bool
eaw_is_mouse(
    eaw_mouse mouse
    ) {
    return mouses[mouse] == EAW_STATE_PRESSED || mouses[mouse] == EAW_STATE_HELD;
}
bool
eaw_is_mouse_pressed(
    eaw_mouse mouse
    ) {
    return mouses[mouse] == EAW_STATE_PRESSED;
}
bool
eaw_is_mouse_released(
    eaw_mouse mouse
    ) {
    return mouses[mouse] == EAW_STATE_RELEASED;
}

bool
eaw_tick_is_key(
    eaw_key key
    ) {
    return tick_keys[key] == EAW_STATE_PRESSED || tick_keys[key] == EAW_STATE_HELD;
}
bool
eaw_tick_is_key_pressed(
    eaw_key key
    ) {
    return tick_keys[key] == EAW_STATE_PRESSED;
}
bool
eaw_tick_is_key_released(
    eaw_key key
    ) {
    return tick_keys[key] == EAW_STATE_RELEASED;
}

bool
eaw_tick_is_mouse(
    eaw_mouse mouse
    ) {
    return tick_mouses[mouse] == EAW_STATE_PRESSED || tick_mouses[mouse] == EAW_STATE_HELD;
}
bool
eaw_tick_is_mouse_pressed(
    eaw_mouse mouse
    ) {
    return tick_mouses[mouse] == EAW_STATE_PRESSED;
}
bool
eaw_tick_is_mouse_released(
    eaw_mouse mouse
    ) {
    return tick_mouses[mouse] == EAW_STATE_RELEASED;
}


eaw_mouse_mode cur_mouse_mode = EAW_MOUSE_MODE_NORMAL;

void
eaw_set_mouse_mode(
    eaw_mouse_mode mode
    ) {
    cur_mouse_mode = mode;

    switch (mode) {
    case EAW_MOUSE_MODE_NORMAL: glfwSetInputMode(eaw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); break;
    case EAW_MOUSE_MODE_HIDDEN: glfwSetInputMode(eaw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); break;
    case EAW_MOUSE_MODE_LOCKED: glfwSetInputMode(eaw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); break;
    }
}


void
_scroll_cb(
    GLFWwindow* window,
    double xoff,
    double yoff
    ) {
    eaw_mouse_scroll_x = xoff;
    eaw_mouse_scroll_y = yoff;
    eaw_mouse_scroll_x64 = xoff;
    eaw_mouse_scroll_y64 = yoff;
}

void
_char_cb(
    GLFWwindow* window,
    unsigned int codepoint
    ) {
    if (eaw_text_input_chars >= 1024) return; // no clue how the fuck this would happen

    eaw_text_input[eaw_text_input_chars] = codepoint;
    ++eaw_text_input_chars;

    if (eaw_tick_text_input_chars >= 1024) return;

    eaw_tick_text_input[eaw_tick_text_input_chars] = codepoint;
    ++eaw_tick_text_input_chars;
}

void
eaw_input_init(
    void
    ) {
    glfwSetScrollCallback(eaw_window, _scroll_cb);
    glfwSetCharCallback(eaw_window, _char_cb);
}

#define up(upk, upks, upm) do{ \
    eaw_key_state prev; \
\
    upk(ESCAPE, ESCAPE); \
    upk(CAPS, CAPS_LOCK);\
    upk(SPACE, SPACE);\
    upk(TAB, TAB);\
    upk(BACKSPACE, BACKSPACE);\
    upk(DELETE, DELETE);\
    upk(HOME, HOME);\
    upk(END, END);\
    upk(PAGE_UP, PAGE_UP);\
    upk(PAGE_DOWN, PAGE_DOWN);\
    upk(INSERT, INSERT);\
    upk(ENTER, ENTER);\
\
    upk(LSHIFT, LEFT_SHIFT);\
    upk(RSHIFT, RIGHT_SHIFT);\
    upk(LCTRL, LEFT_CONTROL);\
    upk(RCTRL, RIGHT_CONTROL);\
    upk(LALT, LEFT_ALT);\
    upk(RALT, RIGHT_ALT);\
    upk(LSUPER, LEFT_SUPER);\
    upk(RSUPER, RIGHT_SUPER);\
\
    upk(UP, UP);\
    upk(DOWN, DOWN);\
    upk(LEFT, LEFT);\
    upk(RIGHT, RIGHT);\
\
    for (int i = EAW_KEY_A; i <= EAW_KEY_Z; ++i) upks(i, GLFW_KEY_A + i - EAW_KEY_A);\
    for (int i = EAW_KEY_1; i <= EAW_KEY_9; ++i) upks(i, GLFW_KEY_1 + i - EAW_KEY_1);\
        upk(0, 0);\
    for (int i = EAW_KEY_F1; i <= EAW_KEY_F12; ++i) upks(i, GLFW_KEY_F1 + i - EAW_KEY_F1);\
\
    upm(LEFT, LEFT);\
    upm(RIGHT, RIGHT);\
    upm(MIDDLE, MIDDLE);\
} while(0)

void
eaw_input_frame(
    void
    ) {
#define frame_upk(key, glfw) frame_upks(EAW_KEY_##key, GLFW_KEY_##glfw)
#define frame_upks(key, glfw) do { \
    prev = keys[key]; \
    keys[key] = glfwGetKey(eaw_window, glfw); \
    if (keys[key] == EAW_STATE_PRESSED && ( prev == EAW_STATE_PRESSED || prev == EAW_STATE_HELD )) keys[key] = EAW_STATE_HELD; \
    if (keys[key] == EAW_STATE_RELEASED && ( prev == EAW_STATE_RELEASED || prev == EAW_STATE_INACTIVE )) keys[key] = EAW_STATE_INACTIVE; \
} while(0)

#define frame_upm(key, glfw) do { \
    prev = mouses[EAW_MOUSE_##key]; \
    mouses[EAW_MOUSE_##key] = glfwGetMouseButton(eaw_window, GLFW_MOUSE_BUTTON_##glfw); \
    if (mouses[EAW_MOUSE_##key] == EAW_STATE_PRESSED && ( prev == EAW_STATE_PRESSED || prev == EAW_STATE_HELD )) mouses[EAW_MOUSE_##key] = EAW_STATE_HELD; \
    if (mouses[EAW_MOUSE_##key] == EAW_STATE_RELEASED && ( prev == EAW_STATE_RELEASED || prev == EAW_STATE_INACTIVE )) mouses[EAW_MOUSE_##key] = EAW_STATE_INACTIVE; \
} while(0)

    up(frame_upk, frame_upks, frame_upm);

    last_mouse_x = eaw_mouse_x64;
    last_mouse_y = eaw_mouse_y64;
    glfwGetCursorPos(eaw_window, &eaw_mouse_x64, &eaw_mouse_y64);
    eaw_mouse_x = eaw_mouse_x64;
    eaw_mouse_y = eaw_mouse_y64;
    eaw_mouse_delta_x64 = eaw_mouse_x64 - last_mouse_x;
    eaw_mouse_delta_y64 = eaw_mouse_y64 - last_mouse_y;
    eaw_mouse_delta_x = eaw_mouse_delta_x64;
    eaw_mouse_delta_y = eaw_mouse_delta_y64;
}

void
eaw_input_tick(
    void
    ) {
    if (!eau_tick_this_frame) return;

#define tick_upk(key, glfw) tick_upks(EAW_KEY_##key, GLFW_KEY_##glfw)
#define tick_upks(key, glfw) do { \
    prev = tick_keys[key]; \
    tick_keys[key] = glfwGetKey(eaw_window, glfw); \
    if (tick_keys[key] == EAW_STATE_PRESSED && ( prev == EAW_STATE_PRESSED || prev == EAW_STATE_HELD )) tick_keys[key] = EAW_STATE_HELD; \
    if (tick_keys[key] == EAW_STATE_RELEASED && ( prev == EAW_STATE_RELEASED || prev == EAW_STATE_INACTIVE )) tick_keys[key] = EAW_STATE_INACTIVE; \
} while(0)

#define tick_upm(key, glfw) do { \
    prev = tick_mouses[EAW_MOUSE_##key]; \
    tick_mouses[EAW_MOUSE_##key] = glfwGetMouseButton(eaw_window, GLFW_MOUSE_BUTTON_##glfw); \
    if (tick_mouses[EAW_MOUSE_##key] == EAW_STATE_PRESSED && ( prev == EAW_STATE_PRESSED || prev == EAW_STATE_HELD )) tick_mouses[EAW_MOUSE_##key] = EAW_STATE_HELD; \
    if (tick_mouses[EAW_MOUSE_##key] == EAW_STATE_RELEASED && ( prev == EAW_STATE_RELEASED || prev == EAW_STATE_INACTIVE )) tick_mouses[EAW_MOUSE_##key] = EAW_STATE_INACTIVE; \
} while(0)

    up(tick_upk, tick_upks, tick_upm);
}
