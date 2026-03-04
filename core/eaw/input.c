#include "input.h"
//#include "../cutil.h"

#include "window.h"

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

double last_mouse_x;
double last_mouse_y;

eaw_key_state keys[EAW_KEY_LAST];
eaw_key_state mouses[EAW_MOUSE_LAST];

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


eaw_mouse_mode cur_mouse_mode = EAW_MOUSE_MODE_NORMAL;

void
eaw_set_mouse_mode(
    eaw_mouse_mode mode
    ) {
    cur_mouse_mode = mode;

    switch (mode) {
    case EAW_MOUSE_MODE_NORMAL: glfwSetInputMode(eaw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    case EAW_MOUSE_MODE_HIDDEN: glfwSetInputMode(eaw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    case EAW_MOUSE_MODE_LOCKED: glfwSetInputMode(eaw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
eaw_input_init(
    void
    ) {
    glfwSetScrollCallback(eaw_window, _scroll_cb);
}

void
eaw_input_frame(
    void
    ) {
    eaw_key_state prev;

#define upk(key, glfw) upks(EAW_KEY_##key, GLFW_KEY_##glfw)
#define upks(key, glfw) do { \
    prev = keys[key]; \
    keys[key] = glfwGetKey(eaw_window, glfw); \
    if (keys[key] == EAW_STATE_PRESSED && ( prev == EAW_STATE_PRESSED || prev == EAW_STATE_HELD )) keys[key] = EAW_STATE_HELD; \
    if (keys[key] == EAW_STATE_RELEASED && ( prev == EAW_STATE_RELEASED || prev == EAW_STATE_INACTIVE )) keys[key] = EAW_STATE_INACTIVE; \
} while(0)

#define upm(key, glfw) do { \
    prev = mouses[EAW_MOUSE_##key]; \
    mouses[EAW_MOUSE_##key] = glfwGetKey(eaw_window, GLFW_MOUSE_BUTTON_##glfw); \
    if (mouses[EAW_MOUSE_##key] == EAW_STATE_PRESSED && ( prev == EAW_STATE_PRESSED || prev == EAW_STATE_HELD )) mouses[EAW_MOUSE_##key] = EAW_STATE_HELD; \
    if (mouses[EAW_MOUSE_##key] == EAW_STATE_RELEASED && ( prev == EAW_STATE_RELEASED || prev == EAW_STATE_INACTIVE )) mouses[EAW_MOUSE_##key] = EAW_STATE_INACTIVE; \
} while(0)

    upk(ESCAPE, ESCAPE);
    upk(CAPS, CAPS_LOCK);
    upk(SPACE, SPACE);
    upk(TAB, TAB);
    upk(BACKSPACE, BACKSPACE);
    upk(DELETE, DELETE);
    upk(HOME, HOME);
    upk(END, END);
    upk(PAGE_UP, PAGE_UP);
    upk(PAGE_DOWN, PAGE_DOWN);
    upk(INSERT, INSERT);

    upk(LSHIFT, LEFT_SHIFT);
    upk(RSHIFT, RIGHT_SHIFT);
    upk(LCTRL, LEFT_CONTROL);
    upk(RCTRL, RIGHT_CONTROL);
    upk(LALT, LEFT_ALT);
    upk(RALT, RIGHT_ALT);
    upk(LSUPER, LEFT_SUPER);
    upk(RSUPER, RIGHT_SUPER);

    upk(UP, UP);
    upk(DOWN, DOWN);
    upk(LEFT, LEFT);
    upk(RIGHT, RIGHT);

    for (int i = EAW_KEY_A; i <= EAW_KEY_Z; ++i) upks(i, GLFW_KEY_A + i - EAW_KEY_A);
    for (int i = EAW_KEY_1; i <= EAW_KEY_9; ++i) upks(i, GLFW_KEY_1 + i - EAW_KEY_1);
        upk(0, 0);
    for (int i = EAW_KEY_F1; i <= EAW_KEY_F12; ++i) upks(i, GLFW_KEY_F1 + i - EAW_KEY_F1);

    upm(LEFT, LEFT);
    upm(RIGHT, RIGHT);
    upm(MIDDLE, MIDDLE);

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
