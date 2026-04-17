#include "glfw.h"

int32_t eaw_glfw_eval_score(void) {
    return 1; // assume glfw works
}

eat_backend_windowing_impl eaw_glfw_impl = {
    .main = {
        //.init = eaw_glfw_init,
        .exit = eaw_glfw_exit,

        .frame = eaw_glfw_frame,
        },
    };
eat_plugin eaw_glfw_plugin = {
    .type = EAT_PLUG_BACKEND_WINDOW,
    .name = "glfw",

    .score = eaw_glfw_eval_score,

    .init = eaw_glfw_init,
    .exit = eaw_glfw_exit,
    };

void
eaw_glfw_init(
    void
    ) {
}
void
eaw_glfw_exit(
    void
    ) {
}

void
eaw_glfw_frame(
    void
    ) {
}
