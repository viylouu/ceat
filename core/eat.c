#include "../eat.h"

int32_t eat_width;
int32_t eat_height;

float eat_time;
float eat_delta;
    double eat_time64;
    double eat_delta64;

void 
eat_init(
    const char* title,
    int32_t width, int32_t height,
    eat_init_opts opts
    ) {
    eaw_init(title, width, height, opts.vsync);
    ear_init();
}

void
eat_stop(
    void
    ) {
    ear_stop();
    eaw_stop();
}

bool
eat_frame(
    void
    ) {
    ear_frame();
    eaw_frame();

    eat_time = eaw_time;
    eat_delta = eaw_delta;
    eat_time64 = eaw_time;
    eat_delta64 = eaw_delta;

    eat_width = eaw_window_width;
    eat_height = eaw_window_height;

    return eaw_is_open();
}
