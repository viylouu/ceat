#include "text.h"
//#include "../cutil.h"

#include "user.h"

void
ear_text(
    ear_texture* atlas,
    char* text,
    float x, float y,
    float scalex, float scaley,
    float col[4]
    ) {
    float ox = 0; float oy = 0;
    float charw = (float)atlas->width / 16;
    float charh = (float)atlas->height / 16;

    for (int i = 0; text[i] != '\0'; ++i) {
        ear_tex(
            atlas,
            ox * charw * scalex + x,
            oy * charh * scaley + y,
            charw * scalex,
            charh * scaley,
            (float)(text[i] >> 4) * charw,
            (float)(text[i] & 0xF) * charh,
            charw, charh,
            col
            );

        ++ox;
        if (text[i] == '\t') ox += 3;
        if (text[i] == '\n') {
            ox = 0;
            ++oy;
        }
    }
}
