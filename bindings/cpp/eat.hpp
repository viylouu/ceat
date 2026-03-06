#pragma once

#include "../../eat.h"

#include <array>
#include <string>

namespace eat{
    static int32_t width;
    static int32_t height;

    static float time;
    static float delta;
    static double time64;
    static double delta64;

    struct init_opts{
        bool vsync = true;
    };

    static void
    init(
        std::string title,
        int32_t width, int32_t height,
        init_opts opts
        );

    static void
    stop(
        void
        );

    static bool
    frame(
        void
        );
};

namespace eaw{};

namespace eau{};

namespace ear{};

static void 
eat::init(
    std::string title,
    int32_t width, int32_t height,
    eat::init_opts opts
    ) {
    eat_init(
        (char*)title.c_str(),
        width, height,
        eat_init_opts{
            .vsync = opts.vsync,
            }
        );
}

static void
eat::stop(
    void
    ) {
    eat_stop();
}

static bool
eat::frame(
    void
    ) {
    bool res = eat_frame();

    eat::width = eat_width;
    eat::height = eat_height;
    eat::time = eat_time;
    eat::delta = eat_delta;
    eat::time64 = eat_time64;
    eat::delta64 = eat_delta64;

    return res;
}
