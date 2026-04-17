#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct eat_backend_windowing_impl{
    struct{
        void (*init)(
            const char* title,
            int32_t width, int32_t height,
            bool vsync
            );
        void (*exit)(
            void
            );

        void (*frame)(
            void
            );
    } main;
} eat_backend_windowing_impl;

extern eat_backend_windowing_impl* eaw_backend;
