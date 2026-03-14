#include "debug.h"

eat_debug_desc debug;
ear_font* debug_font;

void
eat_debug_init(
    void
    ) {
    static const char tex_data[] = {
    #embed "verdana.ttf"
    };

    debug_font = ear_load_truetype_font(tex_data, sizeof(tex_data), NULL);
}

void
eat_debug_stop(
    void
    ) {
    ear_delete_font(debug_font);
}

void
eat_debug_try_do(
    void
    ) {
    if (!eaw_is_key_pressed(debug.key)) return;
}
