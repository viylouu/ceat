#include "text.h"
#include "../../cutil.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "../../../include/stb_truetype.h"

#include "user.h"
#include "../../eau/coll.h"

void
_ear_arena_font_delete(
    void* font
    ) { 
    ear_delete_font(font); 
}

void
_ear_debug_font_window(
    void* font,
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

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

        .deb_obj = eat_debug_add_obj(
            font,
            "font",
            _ear_debug_font_window,
            arena
            ),
        };

    if (arena != NULL) eau_add_to_arena(arena, &font->dest, font, _ear_arena_font_delete);
    return font;
}

ear_font*
ear_load_truetype_font(
    const char* data, size_t data_size,
    eau_arena* arena
    ) {
    (void)data_size;

    ear_font* font = malloc(sizeof(ear_font));
    *font = (ear_font){
        .type = EAR_FONT_TRUETYPE,
        .truetype = (ear_truetype_font){
            .atlas_amt = 0,
            },

        .deb_obj = eat_debug_add_obj(
            font,
            "font",
            _ear_debug_font_window,
            arena
            ),
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
    eat_debug_remove_obj(font->deb_obj);

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
    float scale,
    float col[4],
    eau_align align
    );

void
_ear_bitmap_mono_text_size(
    ear_texture* atlas,
    char* text,
    float scale,
    float* out_width, float* out_height
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
_ear_truetype_text_size(
    ear_font* font,
    char* text,
    float height,
    float* out_width, float* out_height
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
ear_text_size(
    ear_font* font,
    char* text,
    float height,

    float* out_width,
    float* out_height
    ) {
    switch (font->type) {
    case EAR_FONT_BITMAP_MONO:
        _ear_bitmap_mono_text_size(
            font->bitmap_mono.atlas,
            text,
            height / font->bitmap_mono.charh,
            out_width, out_height
            );
        break;
    case EAR_FONT_TRUETYPE:
        _ear_truetype_text_size(
            font,
            text,
            height,
            out_width, out_height
            );
        break;
    }
}


void
_ear_bitmap_mono_text(
    ear_texture* atlas,
    char* text,
    float x, float y,
    float scale,
    float col[4],
    eau_align align
    ) {
    float ox = 0; float oy = 0;
    float charw = atlas->width / 16.f;
    float charh = atlas->height / 16.f;

    float width; float height;
    _ear_bitmap_mono_text_size(
        atlas,
        text,
        scale,
        &width, &height
        );

    eau_rect rect = (eau_rect){ 0,0,width,height, align };
    rect = eau_rect_topleftify(rect);

    for (int i = 0; text[i] != '\0'; ++i) {
        ear_tex(
            atlas,
            ox * charw * scale + x + rect.x,
            oy * charh * scale + y + rect.y,
            charw * scale,
            charh * scale,
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

void
_ear_bitmap_mono_text_size(
    ear_texture* atlas,
    char* text,
    float scale,
    float* out_width, float* out_height
    ) {
    float ox = 0; float oy = 0;
    float charw = atlas->width / 16.f;
    float charh = atlas->height / 16.f;

    float width = 0; float height = 0;

    for (int i = 0; text[i] != '\0'; ++i) {
        ++ox;
        if (text[i] == '\t') ox += 3;
        if (text[i] == '\n') {
            ox = 0;
            ++oy;
        }

        if (ox * charw * scale > width)  width = ox * charw * scale;
        if (oy * charh * scale > height) height = oy * charh * scale;
    }

    if (out_width) *out_width = width;
    if (out_height) *out_height = height;
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
    float scale = stbtt_ScaleForPixelHeight(&font->truetype.info, height);

    float ox = 0; float oy = 0;
    float w; float h;
    _ear_truetype_text_size(
        font,
        text,
        height,
        &w, &h
        );

    struct _ear_truetype_font_atlas* atlas;
    for (uint32_t i = 0; i < font->truetype.atlas_amt; ++i) {
        if (font->truetype.atlases[i].height != height) continue;
        atlas = &font->truetype.atlases[i];
        break;
    }

    eau_rect rect = (eau_rect){ 0,0,w,h, align };
    rect = eau_rect_topleftify(rect);

    for (int i = 0; text[i] != '\0'; ++i) {
        struct _ear_truetype_font_char* ch = &atlas->chars[(uint8_t)text[i]];

        eau_rect indiv = (eau_rect){ 0,0, ch->adw * scale - ch->w, font->lineheight * height - (font->truetype.ascent - font->truetype.descent) * scale, align };
        indiv = eau_rect_topleftify(indiv);

        if (text[i] != '\n' && text[i] != '\t') {
            ear_tex(
                atlas->atlas,
                ox + off_x + rect.x - indiv.x, 
                oy + off_y + font->truetype.ascent * scale + ch->yoff + rect.y - indiv.y,
                ch->w, ch->h,
                ch->x, ch->y, ch->w, ch->h,
                col, EAU_ALIGN_TOP_LEFT
                );
        }

        ox += ch->adw * scale;
        if (text[i] == '\t') ox += ch->adw * scale * 3;
        if (text[i] == '\n') {
            ox = 0;
            oy += font->lineheight * height;
        }
    }
}

void
_ear_truetype_text_size(
    ear_font* font,
    char* text,
    float height,
    float* out_width, float* out_height
    ) {
    // https://github.com/justinmeiners/stb-truetype-example/blob/master/main.c

    bool found = false;
    uint32_t idx = 0;

    for (uint32_t i = 0; i < font->truetype.atlas_amt; ++i) {
        if (font->truetype.atlases[i].height != height) continue;
        found = true;
        idx = i;
        break;
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

    float scale = stbtt_ScaleForPixelHeight(&font->truetype.info, height);

    float ox = 0; float oy = 0;
    float w = 0; float h = 0;

    for (uint32_t i = 0; text[i] != '\0'; ++i) {
        struct _ear_truetype_font_char* ch = &atlas->chars[(uint8_t)text[i]];
        if (!ch->exists && text[i] != '\n') {
            ch->exists = true;

            int ax; int lsb;
            stbtt_GetCodepointHMetrics(&font->truetype.info, text[i], &ax,&lsb);
            ch->adw = ax;

            if (text[i] != '\t') {
                int xoff; int yoff; int wid; int hei;
                char* glyph = (char*)stbtt_GetCodepointBitmap(&font->truetype.info, scale, scale, text[i], &wid,&hei,&xoff,&yoff);
                ch->w = wid + 1;
                ch->h = hei + 1;
                ch->yoff = yoff;

                if (atlas->lastx + ch->w > at_width) {
                    atlas->lastx = 0;
                    atlas->lasty += atlas->curmaxh;
                    atlas->curmaxh = ch->h;
                }

                ch->x = atlas->lastx;
                ch->y = atlas->lasty;

                for (uint32_t y = 0; y < (uint32_t)hei; ++y) for (uint32_t x = 0; x < (uint32_t)wid; ++x) {
                    char a = glyph[y * wid + x];
                    ear_set_texture_color(atlas->atlas, ch->x + x, at_height - (ch->y + y) - 1, (float[4]){ 1,1,1,a/255.f });
                }

                stbtt_FreeBitmap((uint8_t*)glyph, NULL);

                atlas->lastx += ch->w;
                if (ch->h > atlas->curmaxh) atlas->curmaxh = ch->h;

                ear_update_texture(atlas->atlas);
            }
        }

        if (i != 0) {
            float kern = stbtt_GetCodepointKernAdvance(&font->truetype.info, text[i-1], text[i]);
            ox += kern * scale;
        }

        ox += ch->adw * scale;
        if (text[i] == '\t') ox += ch->adw * scale * 3;
        if (text[i] == '\n') {
            ox = 0;
            oy += font->lineheight * height;
        }

        if (ox > w) w = ox;
        if (oy + font->lineheight * height > h) h = oy + font->lineheight * height;
    }

    if (out_width) *out_width = w;
    if (out_height) *out_height = h;
}


void
_ear_debug_font_window(
    void* _font,
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* selected
    ) {
    ear_font* font = _font;

    float offy = 0;
    float off = 16;
    
    char buf[64];

    switch (font->type) {
    case EAR_FONT_BITMAP_MONO: snprintf(buf, sizeof(buf), "type: bitmap-mono"); break;
    case EAR_FONT_TRUETYPE: snprintf(buf, sizeof(buf), "type: truetype"); break;
    }

    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    switch (font->type) {
    case EAR_FONT_BITMAP_MONO:
        snprintf(buf, sizeof(buf), "char width: %.0f, char height: %.0f", font->bitmap_mono.charw, font->bitmap_mono.charh);
        ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
        offy += off;

        uint32_t idx = 0;
        for (eat_debug_ll_obj* it = eat_debug_ll_first; it != NULL; it = it->next) {
            if (it->data == font->bitmap_mono.atlas) break;
            ++idx;
        }

        snprintf(buf, sizeof(buf), "texture %d", idx);

        float width;
        ear_text_size(t.font, buf, 14, &width, NULL);

        bool sel = eau_point_rect(eaw_mouse_x,eaw_mouse_y, (eau_rect){ x,y+offy, width+8,16, EAU_ALIGN_TOP_LEFT });

        ear_rect(x,y+offy, width+8, 16, sel? debug_theme.sel_but_col : debug_theme.but_col, EAU_ALIGN_TOP_LEFT);
        ear_rect(x+2,y+2+offy, width+4, 12, sel? debug_theme.but_col : debug_theme.bg_col, EAU_ALIGN_TOP_LEFT);

        if (sel && eaw_is_mouse_pressed(EAW_MOUSE_LEFT)) *selected = idx;

        ear_text(t.font, buf, x + 4, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);

        offy += 18;

        float sizex = w;
        float sizey = h - offy;

        const float wpaspect = (float)font->bitmap_mono.atlas->width / font->bitmap_mono.atlas->height;
        float winaspect = sizex / sizey;

        width = sizex;
        float height = sizey;
        if (winaspect > wpaspect) width = height * wpaspect;
        else height = width / wpaspect;

        float draw_offx = (sizex - width) * .5;
        float draw_offy = (sizey - height) * .5;

        ear_rect(draw_offx + x, draw_offy + y+offy, width, height, (float[4]){ 0,0,0,1 }, EAU_ALIGN_TOP_LEFT);
        ear_tex(font->bitmap_mono.atlas, draw_offx + x, draw_offy + y+offy, width,height, 0,0,font->bitmap_mono.atlas->width,font->bitmap_mono.atlas->height, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);

        break;
    case EAR_FONT_TRUETYPE:
        if (font->truetype.atlas_amt == 0) {
            ear_text(t.font, "no atlases", x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
        } else {
            snprintf(buf, sizeof(buf), "%d atlases:", font->truetype.atlas_amt);
            ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
            offy += off;

            for (uint32_t i = 0; i < font->truetype.atlas_amt; ++i) {
                uint32_t idx = 0;
                for (eat_debug_ll_obj* it = eat_debug_ll_first; it != NULL; it = it->next) {
                    if (it->data == font->truetype.atlases[i].atlas) break;
                    ++idx;
                }

                snprintf(buf, sizeof(buf), "texture %d", idx);

                float width;
                ear_text_size(t.font, buf, 14, &width, NULL);

                bool sel = eau_point_rect(eaw_mouse_x,eaw_mouse_y, (eau_rect){ x,y+offy, width+8,16, EAU_ALIGN_TOP_LEFT });

                ear_rect(x,y+offy, width+8, 16, sel? debug_theme.sel_but_col : debug_theme.but_col, EAU_ALIGN_TOP_LEFT);
                ear_rect(x+2,y+2+offy, width+4, 12, sel? debug_theme.but_col : debug_theme.bg_col, EAU_ALIGN_TOP_LEFT);

                if (sel && eaw_is_mouse_pressed(EAW_MOUSE_LEFT)) *selected = idx;

                ear_text(t.font, buf, x + 4, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);

                offy += 18;
            }
        }
        break;
    }
}
