#pragma once

#include "../impl.h"
#include "../../../eat.h"

extern eat_backend_windowing_impl eaw_glfw_impl;
extern eat_plugin eaw_glfw_plugin;

void
eaw_glfw_init(
    void
    );
void
eaw_glfw_exit(
    void
    );

void
eaw_glfw_frame(
    void
    );
