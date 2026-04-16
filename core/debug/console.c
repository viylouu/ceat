#include "console.h"

#include "debug.h"
#include "../../eat.h"
#include "../eaw/window.h"

eat_console_desc console;

float text_repeat_wait = .5;
float text_repeat_delta = 1./24;
float last_hold_time = 0;
bool repeating = false;

float underscore_tick = .75;
float last_tick_time = 0;
bool underscore = true;

void
eat_console_init(
    void
    ) {
}

void
eat_console_stop(
    void
    ) {
}

void
eat_console_try_do(
    void
    ) {
    if (!eaw_is_key_pressed(console.key)) return;

    ear_frame(false);
    eaw_frame();

    char buf[256];
    uint32_t buf_amt = 0;

    underscore = true;
    last_tick_time = eaw_time;

    while (true) {
        eat_width = _eaw_window_width;
        eat_height = _eaw_window_height;

        ear_rect(0, eat_height/2., eat_width, debug_theme.font->lineheight * 16, debug_theme.bg_col, EAU_ALIGN_MID_LEFT);

        for (uint32_t i = 0; i < eaw_text_input_chars; ++i) {
            buf[buf_amt] = eaw_text_input[i];
            ++buf_amt;
        }

        if (buf_amt != 0) {
            if (eaw_is_key_pressed(EAW_KEY_BACKSPACE)) { 
                --buf_amt; 
                last_hold_time = eaw_time; 
                repeating = false; 
            } else if (eaw_is_key(EAW_KEY_BACKSPACE)) {
                if (eaw_time - last_hold_time > text_repeat_wait) {
                    if (!repeating) repeating = true;
                    else {
                        --buf_amt;
                        last_hold_time = eaw_time;
                    }
                }
            }
        }

        if (eaw_time - last_tick_time > underscore_tick) {
            underscore = !underscore;
            last_tick_time = eaw_time;
        }

        if (underscore) buf[buf_amt] = '_';
        else buf[buf_amt] = 0;
        buf[buf_amt+1] = 0;

        ear_text(debug_theme.font, buf, 4, eat_height/2., 14, debug_theme.text_col, EAU_ALIGN_MID_LEFT);

        ear_frame(false);
        eaw_frame();

        if (!eaw_is_open()) return;

        if (eaw_is_key_pressed(EAW_KEY_ENTER)) {
            buf[buf_amt] = 0;
            console.command_solver(buf);
            return;
        }
    }
}
