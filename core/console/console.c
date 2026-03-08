#include "console.h"

eat_console_desc console;

ear_texture* font;

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
    static const uint8_t tex_data[] = {
    #embed "font.png"
    };

    font = ear_load_texture((ear_texture_desc){
            .filter = EAR_FILTER_NEAREST,
            .type = EAR_TEX_COLOR,
            .wrap = EAR_WRAP_REPEAT,
        }, tex_data, sizeof(tex_data), NULL);
}

void
eat_console_stop(
    void
    ) {
    ear_delete_texture(font);
}

void
eat_console_try_do(
    void
    ) {
    if (!eaw_is_key_pressed(console.key)) return;

    ear_frame();
    eaw_frame();

    char buf[256];
    uint32_t buf_amt = 0;

    underscore = true;
    last_tick_time = eaw_time;

    while (true) {
        eat_width = eaw_window_width;
        eat_height = eaw_window_height;

        ear_rect(0, eat_height/2., eat_width, font->height/16.*3, (float[4]){ 0,0,0,1 }, EAR_ALIGN_MID_LEFT);

        for (uint32_t i = 0; i < eaw_text_input_chars; ++i) {
            buf[buf_amt] = eaw_text_input[i];
            ++buf_amt;
        }

        if (buf_amt != 0) {
            if (eaw_is_key_pressed(EAW_KEY_BACKSPACE)) { --buf_amt; last_hold_time = eaw_time; repeating = false; }
            if (eaw_is_key(EAW_KEY_BACKSPACE)) {
                if (!repeating) {
                    if (eaw_time - last_hold_time > text_repeat_wait)
                        repeating = true;
                } else {
                    if (eaw_time - last_hold_time > text_repeat_delta) {
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

        ear_text(font, buf, 0, eat_height/2., 3,3, (float[4]){ 1,1,1,1 }, EAR_ALIGN_MID_LEFT);

        ear_frame();
        eaw_frame();

        if (!eaw_is_open()) return;

        if (eaw_is_key_pressed(EAW_KEY_ENTER)) {
            buf[buf_amt] = 0;
            console.command_solver(buf);
            return;
        }
    }
}
