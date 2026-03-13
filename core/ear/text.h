#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../../include/stb_truetype.h"

#include "texture.h"
#include "../eau/coll.h"

typedef struct ear_font ear_font;

typedef struct ear_bitmap_mono_font ear_bitmap_mono_font;
typedef struct ear_truetype_font ear_truetype_font;

//typedef enum ear_font_type ear_font_type;

ear_font*
ear_load_bitmap_mono_font(
    const char* data, size_t data_size,
    eau_arena* arena
    );

ear_font*
ear_load_truetype_font(
    const char* data, size_t data_size,
    eau_arena* arena
    );

void
ear_delete_font(
    ear_font* font
    );

void
ear_text(
    ear_font* font,
    char* text,
    float x, float y,
    float height,
    float col[4],
    eau_align align
    );


typedef enum ear_font_type{
    EAR_FONT_BITMAP_MONO,
    EAR_FONT_TRUETYPE,
} ear_font_type;

struct ear_bitmap_mono_font{
    ear_texture* atlas;

    float charw; float charh;
};

struct _ear_truetype_font_char{
    bool exists;
    float x; float y; 
    float w; float h;
    float adw; float yoff;
};

struct _ear_truetype_font_atlas{
    ear_texture* atlas;
    struct _ear_truetype_font_char chars[256];
    float lastx; float lasty; float curmaxh;
    uint8_t* pixbuf;
    float height;
};

struct ear_truetype_font{
    struct _ear_truetype_font_atlas* atlases;
    uint32_t atlas_amt;

    stbtt_fontinfo info;
    float ascent;
    float descent;
    float linegap;
};

struct ear_font{
    float lineheight;

    ear_font_type type;
    union{
        ear_bitmap_mono_font bitmap_mono;
        ear_truetype_font truetype;
    };

    eau_destructor* dest;
};
