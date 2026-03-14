#include "texture.h"
#include "../cutil.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"

#include "gl.h"

GLenum 
_TYPECONV_texture_filter(
    ear_texture_filter filter
    );

GLenum
_TYPECONV_texture_wrap(
    ear_texture_wrap wrap
    );

GLenum
_TYPECONV_texture_type_as_intf(
    ear_texture_type type
    );

GLenum
_TYPECONV_texture_type_as_f(
    ear_texture_type type
    );

GLenum
_TYPECONV_texture_type_as_type(
    ear_texture_type type
    );


void
_ear_arena_texture_delete(
    void* tex
    ) { 
    ear_delete_texture(tex); 
}

ear_texture*
ear_create_texture(
    ear_texture_desc desc,
    uint8_t pixels[],
    uint32_t width, uint32_t height,
    eau_arena* arena
    ) {
    ear_texture* tex = malloc(sizeof(ear_texture));
    *tex = (ear_texture){
        .desc = desc,
        .width = width, .height = height,

        .pixels = pixels,
        .stbi_pixels = false,
        };

    gl.genTextures(1, &tex->id);
    eat_assert(tex->id != 0, "failed to create opengl texture!");

    gl.bindTexture(GL_TEXTURE_2D, tex->id);

    GLenum sampling = _TYPECONV_texture_filter(tex->desc.filter);
    GLenum wrap = _TYPECONV_texture_wrap(tex->desc.wrap);

    gl.texParameterI(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampling);
    gl.texParameterI(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampling);
    gl.texParameterI(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    gl.texParameterI(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    gl.texParameterFv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, tex->desc.wrap_color);

    gl.texImage2D(
        GL_TEXTURE_2D,
        0,
        _TYPECONV_texture_type_as_intf(tex->desc.type),
        width, height,
        0,
        _TYPECONV_texture_type_as_f(tex->desc.type),
        _TYPECONV_texture_type_as_type(tex->desc.type),
        pixels
        );

    if (arena != NULL) eau_add_to_arena(arena, &tex->dest, tex, _ear_arena_texture_delete);
    return tex;
}

ear_texture*
ear_load_texture(
    ear_texture_desc desc,
    const char* data, size_t data_size,
    eau_arena* arena
    ) {
    int32_t width;
    int32_t height;
    int32_t chans;
    stbi_set_flip_vertically_on_load(1);
    uint8_t* pixels = stbi_load_from_memory((const uint8_t*)data, data_size, &width, &height, &chans, 4);
    eat_assert(pixels != NULL, "failed to load image data!");

    ear_texture* tex = ear_create_texture(desc, pixels, width, height, arena);
    tex->stbi_pixels = true;
    return tex;
}

void
ear_delete_texture(
    ear_texture* tex
    ) {
    gl.deleteTextures(1, &tex->id);
    if (tex->stbi_pixels) stbi_image_free(tex->pixels);

    if (tex->dest != NULL) tex->dest->data = NULL;
    free(tex);
}

void
ear_bind_texture(
    ear_texture* tex,
    uint32_t slot
    ) {
    gl.activeTexture(GL_TEXTURE0 + slot);
    gl.bindTexture(GL_TEXTURE_2D, tex->id);
    gl.uniform1i(slot, slot);
}

void
ear_resize_texture(
    ear_texture* tex,
    uint32_t width, uint32_t height
    ) {
    tex->width = width;
    tex->height = height;

    gl.bindTexture(GL_TEXTURE_2D, tex->id);

    gl.texImage2D(
        GL_TEXTURE_2D,
        0,
        _TYPECONV_texture_type_as_intf(tex->desc.type),
        width, height,
        0,
        _TYPECONV_texture_type_as_f(tex->desc.type),
        _TYPECONV_texture_type_as_type(tex->desc.type),
        tex->pixels
        );
}


void
ear_get_texture_color(
    ear_texture* tex,
    uint32_t x, uint32_t y,
    float (*out)[4]
    ) {
    uint32_t i = (x + y * tex->width) * 4;
    (*out)[0] = (float)tex->pixels[i + 0] / 255.f;
    (*out)[1] = (float)tex->pixels[i + 1] / 255.f;
    (*out)[2] = (float)tex->pixels[i + 2] / 255.f;
    (*out)[3] = (float)tex->pixels[i + 3] / 255.f;
}

void
ear_set_texture_color(
    ear_texture* tex,
    uint32_t x, uint32_t y,
    float col[4]
    ) {
    uint32_t i = (x + y * tex->width) * 4;
    tex->pixels[i + 0] = col[0] * 255;
    tex->pixels[i + 1] = col[1] * 255;
    tex->pixels[i + 2] = col[2] * 255;
    tex->pixels[i + 3] = col[3] * 255;
}

void
ear_update_texture(
    ear_texture* tex
    ) {
    gl.bindTexture(GL_TEXTURE_2D, tex->id);

    gl.texSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,0,
        tex->width, tex->height,
        _TYPECONV_texture_type_as_f(tex->desc.type),
        _TYPECONV_texture_type_as_type(tex->desc.type),
        tex->pixels
        );

    gl.bindTexture(GL_TEXTURE_2D, 0);
}




GLenum 
_TYPECONV_texture_filter(
    ear_texture_filter filter
    ) {
    switch (filter) {
    case EAR_FILTER_NEAREST: return GL_NEAREST;
    case EAR_FILTER_LINEAR:  return GL_LINEAR;
    }

    eat_unreachable();
}

GLenum
_TYPECONV_texture_wrap(
    ear_texture_wrap wrap
    ) {
    switch (wrap) {
    case EAR_WRAP_REPEAT: return GL_REPEAT;
    case EAR_WRAP_CLAMP:  return GL_CLAMP_TO_EDGE;
    case EAR_WRAP_COLOR:  return GL_CLAMP_TO_BORDER;
    }

    eat_unreachable();
}

GLenum
_TYPECONV_texture_type_as_intf(
    ear_texture_type type
    ) {
    switch (type) {
    case EAR_TEX_COLOR: return GL_RGBA8;
    case EAR_TEX_DEPTH: return GL_DEPTH_COMPONENT24;
    case EAR_TEX_HDR:   return GL_RGBA16F;
    case EAR_TEX_HDR32: return GL_RGBA32F;
    }

    eat_unreachable();
}

GLenum
_TYPECONV_texture_type_as_f(
    ear_texture_type type
    ) {
    switch (type) {
    case EAR_TEX_COLOR: return GL_RGBA;
    case EAR_TEX_DEPTH: return GL_DEPTH_COMPONENT;
    case EAR_TEX_HDR:   return GL_RGBA;
    case EAR_TEX_HDR32: return GL_RGBA;
    }

    eat_unreachable();
}

GLenum
_TYPECONV_texture_type_as_type(
    ear_texture_type type
    ) {
    switch (type) {
    case EAR_TEX_COLOR: return GL_UNSIGNED_BYTE;
    case EAR_TEX_DEPTH: return GL_UNSIGNED_INT;
    case EAR_TEX_HDR:   return GL_HALF_FLOAT;
    case EAR_TEX_HDR32: return GL_FLOAT;
    }

    eat_unreachable();
}
