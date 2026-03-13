#include "text.h"
#include "../cutil.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "../../include/stb_truetype.h"

#include "user.h"

void
_ear_arena_font_delete(
    void* font
    ) { 
    ear_delete_font(font); 
}

ear_font*
ear_load_bitmap_mono_font(
    const char* data, size_t data_size,
    eau_arena* arena
    ) {
    ear_texture* atlas = ear_load_texture((ear_texture_desc){
        .type = EAR_TEX_COLOR,
        .filter = EAR_FILTER_NEAREST,
        .wrap = EAR_WRAP_REPEAT,
        }, data, data_size, arena);

    ear_font* font = malloc(sizeof(ear_font));
    *font = (ear_font){
        .type = EAR_FONT_BITMAP_MONO,
        .bitmap_mono = (ear_bitmap_mono_font){
            .atlas = atlas,
            .charw = atlas->width / 16.f,
            .charh = atlas->height / 16.f,
            },
        };

    if (arena != NULL) eau_add_to_arena(arena, &font->dest, font, _ear_arena_font_delete);
    return font;
}

ear_font*
ear_load_truetype_font(
    const char* data, size_t data_size,
    eau_arena* arena
    ) {
    ear_font* font = malloc(sizeof(ear_font));
    *font = (ear_font){
        .type = EAR_FONT_TRUETYPE,
        .truetype = (ear_truetype_font){
            .atlas_amt = 0,
            },
        };

    eat_assert(stbtt_InitFont(&font->truetype.info, (uint8_t*)data, 0), "failed to load font!");

    int ascent; int descent; int linegap;
    stbtt_GetFontVMetrics(&font->truetype.info, &ascent, &descent, &linegap);
    font->truetype.ascent = ascent;
    font->truetype.descent = descent;
    font->truetype.linegap = linegap;

    float scale = stbtt_ScaleForPixelHeight(&font->truetype.info, 1);

    font->lineheight = (ascent - descent + linegap) * scale;

    if (arena != NULL) eau_add_to_arena(arena, &font->dest, font, _ear_arena_font_delete);
    return font;
}

void
ear_delete_font(
    ear_font* font
    ) {
    switch (font->type) {
    case EAR_FONT_TRUETYPE: 
        for (uint32_t i = 0; i < font->truetype.atlas_amt; ++i) {
            struct _ear_truetype_font_atlas* atlas = &font->truetype.atlases[i];

            if (font->dest == NULL) ear_delete_texture(atlas->atlas);
            free(atlas->pixbuf);
        }
        break;
    default:
    }

    if (font->dest != NULL) font->dest->data = NULL;
    free(font);
}

void
_ear_bitmap_mono_text(
    ear_texture* atlas,
    char* text,
    float x, float y,
    float scalex, float scaley,
    float col[4],
    eau_align align
    );

void
_ear_truetype_text(
    ear_font* font,
    char* text,
    float x, float y,
    float height,
    float col[4],
    eau_align align
    );

void
ear_text(
    ear_font* font,
    char* text,
    float x, float y,
    float height,
    float col[4],
    eau_align align
    ) {
    switch (font->type) {
    case EAR_FONT_BITMAP_MONO: 
        _ear_bitmap_mono_text(
            font->bitmap_mono.atlas,
            text,
            x, y,
            height / font->bitmap_mono.charh,
            height / font->bitmap_mono.charh,
            col,
            align
            );
        break;
    case EAR_FONT_TRUETYPE:
        _ear_truetype_text(
            font,
            text,
            x, y,
            height,
            col,
            align
            );
        break;
    }
}


void
_ear_bitmap_mono_text(
    ear_texture* atlas,
    char* text,
    float x, float y,
    float scalex, float scaley,
    float col[4],
    eau_align align
    ) {
    float ox = 0; float oy = 0;
    float charw = atlas->width / 16.f;
    float charh = atlas->height / 16.f;

    float width = 0; float height = 0;

    for (int i = 0; text[i] != '\0'; ++i) {
        if (align == EAU_ALIGN_TOP_LEFT) {
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
                EAU_ALIGN_TOP_LEFT
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

    if (align != EAU_ALIGN_TOP_LEFT) {
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

void
_ear_truetype_text(
    ear_font* font,
    char* text,
    float off_x, float off_y,
    float height,
    float col[4],
    eau_align align
    ) {
    bool found = false;
    uint32_t idx = 0;

    for (uint32_t i = 0; i < font->truetype.atlas_amt; ++i) {
        if (font->truetype.atlases[i].height != height) continue;
        found = true;
        idx = i;
    }

    struct _ear_truetype_font_atlas* atlas;
    if (font->truetype.atlas_amt != 0) atlas = &font->truetype.atlases[idx];

    static const uint32_t at_width = 512;
    static const uint32_t at_height = 512;

    if (!found) {
        ++font->truetype.atlas_amt;
        font->truetype.atlases = realloc(font->truetype.atlases, sizeof(struct _ear_truetype_font_atlas) * font->truetype.atlas_amt);
        idx = font->truetype.atlas_amt-1;
        atlas = &font->truetype.atlases[idx];

        atlas->pixbuf = malloc(sizeof(char) * at_width * at_height * 4);
        atlas->atlas = ear_create_texture((ear_texture_desc){
            .type = EAR_TEX_COLOR,
            .filter = EAR_FILTER_NEAREST,
            .wrap = EAR_WRAP_REPEAT,
            }, atlas->pixbuf, at_width,at_height, font->dest == NULL? NULL : font->dest->arena);
        atlas->height = height;
    }

    // https://github.com/justinmeiners/stb-truetype-example/blob/master/main.c

    float ox = 0; float oy = 0;
    float w = 0; float h = 0;

    float scale = stbtt_ScaleForPixelHeight(&font->truetype.info, height);

    for (uint32_t i = 0; text[i] != '\0'; ++i) {
        struct _ear_truetype_font_char* ch = &atlas->chars[text[i]];
        if (!ch->exists) {
            ch->exists = true;

            int ax; int lsb;
            stbtt_GetCodepointHMetrics(&font->truetype.info, text[i], &ax,&lsb);
            ch->adw = ax;

            int xoff; int yoff; int wid; int hei;
            char* glyph = (char*)stbtt_GetCodepointBitmap(&font->truetype.info, scale, scale, text[i], &wid,&hei,&xoff,&yoff);
            ch->w = wid;
            ch->h = hei;
            ch->yoff = yoff;

            if (atlas->lastx + ch->w > at_width) {
                atlas->lastx = 0;
                atlas->lasty += atlas->curmaxh;
                atlas->curmaxh = ch->h;
            }

            ch->x = atlas->lastx;
            ch->y = atlas->lasty;

            for (uint32_t y = 0; y < hei; ++y) for (uint32_t x = 0; x < wid; ++x) {
                char a = glyph[y * wid + x];
                ear_set_texture_color(atlas->atlas, ch->x + x, at_height - (ch->y + y) - 1, (float[4]){ 1,1,1,a/255.f });
            }

            stbtt_FreeBitmap((uint8_t*)glyph, NULL);

            atlas->lastx += ch->w;
            if (ch->h > atlas->curmaxh) atlas->curmaxh = ch->h;

            ear_update_texture(atlas->atlas);
        }

        if (i != 0) {
            float kern = stbtt_GetCodepointKernAdvance(&font->truetype.info, text[i-1], text[i]);
            ox += kern * scale;
        }

        if (align == EAU_ALIGN_TOP_LEFT && text[i] != '\n' && text[i] != '\t') {
            ear_tex(
                atlas->atlas,
                ox + off_x, 
                oy + off_y + font->truetype.ascent * scale + ch->yoff,
                ch->w, ch->h,
                ch->x, ch->y, ch->w, ch->h,
                col, EAU_ALIGN_TOP_LEFT
                );
        }

        ox += ch->adw * scale;
        if (text[i] == '\t') ox += ch->adw * scale * 3;
        if (text[i] == '\n') {
            ox = 0;
            oy += font->lineheight * scale;
        }

        if (ox > w) w = ox;
        if (oy + font->lineheight * scale > h) h = oy + font->lineheight * scale;
    }

    ox = 0; oy = 0;

    if (align != EAU_ALIGN_TOP_LEFT) {
        eau_rect rect = (eau_rect){ 0,0,w,h, align };
        _CONV_topleftify(&rect);

        for (int i = 0; text[i] != '\0'; ++i) {
            struct _ear_truetype_font_char* ch = &atlas->chars[text[i]];

            eau_rect indiv = (eau_rect){ 0,0, ch->adw * scale - ch->w, font->lineheight * scale - (font->truetype.ascent - font->truetype.descent) * scale, align };
            _CONV_topleftify(&indiv);

            ear_tex(
                atlas->atlas,
                ox + off_x + rect.x - indiv.x, 
                oy + off_y + font->truetype.ascent * scale + ch->yoff + rect.y - indiv.y,
                ch->w, ch->h,
                ch->x, ch->y, ch->w, ch->h,
                col, EAU_ALIGN_TOP_LEFT
                );

            ox += ch->adw * scale;
            if (text[i] == '\t') ox += ch->adw * scale * 3;
            if (text[i] == '\n') {
                ox = 0;
                oy += font->lineheight * scale;
            }
        }
    }
}
