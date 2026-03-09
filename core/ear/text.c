#include "text.h"
//#include "../cutil.h"

#include "../eau/coll.h"

void
ear_text(
    ear_texture* atlas,
    char* text,
    float x, float y,
    float scalex, float scaley,
    float col[4],
    ear_align align
    ) {
    float ox = 0; float oy = 0;
    float charw = (float)atlas->width / 16;
    float charh = (float)atlas->height / 16;

    float width = 0; float height = 0;

    for (int i = 0; text[i] != '\0'; ++i) {
        if (align == EAR_ALIGN_TOP_LEFT) {
            ear_tex(
                atlas,
                ox * charw * scalex + x,
                oy * charh * scaley + y,
                charw * scalex,
                charh * scaley,
                (float)(text[i] >> 4) * charw,
                (float)(text[i] & 0xF) * charh,
                charw, charh,
                col,
                EAR_ALIGN_TOP_LEFT
                );
        }

        ++ox;
        if (text[i] == '\t') ox += 3;
        if (text[i] == '\n') {
            ox = 0;
            ++oy;
        }

        if (ox * charw * scalex > width)  width = ox * charw * scalex;
        if (oy * charh * scaley > height) height = oy * charh * scaley;
    }

    ox = 0; oy = 0;

    if (align != EAR_ALIGN_TOP_LEFT) {
        eau_rect rect = (eau_rect){ 0,0,width,height, align };
        _CONV_topleftify(&rect);

        for (int i = 0; text[i] != '\0'; ++i) {
            ear_tex(
                atlas,
                ox * charw * scalex + x + rect.x,
                oy * charh * scaley + y + rect.y,
                charw * scalex,
                charh * scaley,
                (float)(text[i] >> 4) * charw,
                (float)(text[i] & 0xF) * charh,
                charw, charh,
                col,
                align
                );

            ++ox;
            if (text[i] == '\t') ox += 3;
            if (text[i] == '\n') {
                ox = 0;
                ++oy;
            }
        }
    }
}
