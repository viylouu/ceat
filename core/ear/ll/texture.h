#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../eau/arena.h"
#include "../../debug/debug.h"

#include "bindset.h"

typedef struct ear_texture ear_texture;
typedef struct ear_texture_desc ear_texture_desc;

//typedef enum ear_texture_filter ear_texture_filter;
//typedef enum ear_texture_type ear_texture_type;
//typedef enum ear_texture_wrap ear_texture_wrap;


ear_texture*
ear_create_texture(
    ear_texture_desc desc,
    uint8_t pixels[],
    uint32_t width, uint32_t height,
    eau_arena* arena
    );

ear_texture*
ear_load_texture(
    ear_texture_desc desc,
    const char* data, size_t data_size,
    eau_arena* arena
    );

void
ear_delete_texture(
    ear_texture* texture
    );

void
ear_resize_texture(
    ear_texture* texture,
    uint8_t new_pixels[],
    uint32_t width, uint32_t height
    );


void
ear_get_texture_color(
    ear_texture* texture,
    uint32_t x, uint32_t y,
    float (*out)[4]
    );

void
ear_set_texture_color(
    ear_texture* texture,
    uint32_t x, uint32_t y,
    float col[4]
    );

void
ear_update_texture(
    ear_texture* texture
    );


typedef enum ear_texture_filter{
    EAR_FILTER_NEAREST,
    EAR_FILTER_LINEAR,
} ear_texture_filter;

typedef enum ear_texture_type{
    EAR_TEX_COLOR,
    EAR_TEX_DEPTH,
    EAR_TEX_HDR,
    EAR_TEX_HDR32,
} ear_texture_type;

typedef enum ear_texture_wrap{
    EAR_WRAP_REPEAT,
    EAR_WRAP_MIRROR,
    EAR_WRAP_CLAMP,
    //EAR_WRAP_COLOR,
} ear_texture_wrap;

struct ear_texture_desc{
    ear_texture_filter filter;
    ear_texture_type type;
    ear_texture_wrap wrap;
        //float wrap_color[4]; // vulkan doesent support arbitrary
    bool anisotropy;
};

struct ear_texture{
    void* vk;

    uint8_t* pixels;
        bool stbi_pixels;
        bool self_pixels;
        uint32_t perpix;

    uint32_t width;
    uint32_t height;

    ear_bindset* hl_bindset;

    ear_texture_desc desc;

    eau_destructor* dest;
    eat_debug_ll_obj* deb_obj;
};
