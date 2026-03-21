#pragma once

#include <stdint.h>
#include <stdbool.h>

extern int32_t _eaw_window_width;
extern int32_t _eaw_window_height;

typedef struct GLFWwindow GLFWwindow;
extern GLFWwindow* _eaw_glfw_window;

void
eaw_window_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    );

void 
eaw_window_stop(
    void
    );

void
eaw_window_frame(
    void
    );

bool
eaw_window_is_open(
    void
    );
