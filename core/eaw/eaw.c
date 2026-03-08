#include "eaw.h"
//#include "../cutil.h"

#include <GLFW/glfw3.h>

double eaw_time;
    double eaw_last_time;
double eaw_delta;

void
eaw_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    ) {
    eaw_window_init(title, width, height, vsync);
    eaw_input_init();

    eaw_time = glfwGetTime();
    eaw_last_time = 0;
}

void
eaw_stop(
    void
    ) {
    eaw_window_stop();
}

void
eaw_frame(
    void
    ) {
    eaw_mouse_scroll_x = 0;
    eaw_mouse_scroll_y = 0;
    eaw_mouse_scroll_x64 = 0;
    eaw_mouse_scroll_y64 = 0;

    eaw_window_frame();
    
    eaw_text_input_chars = 0;

    glfwPollEvents();

    eaw_input_frame();

    eaw_time = glfwGetTime();
    eaw_delta = eaw_time - eaw_last_time;
    eaw_last_time = eaw_time;
}

bool
eaw_is_open(
    void
    ) {
    return eaw_window_is_open();
}
