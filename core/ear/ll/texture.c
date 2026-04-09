#include "texture.h"
#include "../../cutil.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../include/stb_image.h"

//#include "text.h"
//#include "user.h"
//#include "gl.h"

#include "rendering/vulkan/eng/texture.h"

void
_ear_arena_texture_delete(
    void* tex
    ) { 
    ear_delete_texture(tex); 
}

void
_ear_debug_texture_window(
    void* tex,
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

ear_texture*
ear_create_texture(
    ear_texture_desc desc,
    uint8_t pixels[],
    uint32_t width, uint32_t height,
    eau_arena* arena
    ) {
    ear_texture* tex = malloc(sizeof(ear_texture));
    
    switch (desc.type) {
    case EAR_TEX_COLOR: tex->perpix = sizeof(uint8_t)  * 4; break;
    case EAR_TEX_DEPTH: tex->perpix = sizeof(uint8_t)  * 4; break;
    case EAR_TEX_HDR:   tex->perpix = sizeof(uint16_t) * 4; break;
    case EAR_TEX_HDR32: tex->perpix = sizeof(uint32_t) * 4; break;
    default: eat_unreachable();
    }

    tex->pixels      = pixels;
    tex->self_pixels = false;
    if (pixels == NULL) {
        tex->self_pixels = true;

        uint32_t imgsize = width * height * tex->perpix;
        tex->pixels = malloc(imgsize);
    }

    *tex = (ear_texture){
        .vk = ear_vk_create_texture(desc, &tex->pixels, width, height),

        .desc = desc,
        .width = width, .height = height,

        .hl_bindset = NULL,

        .pixels = tex->pixels,
        .stbi_pixels = false,
        .self_pixels = tex->self_pixels,

        .perpix = tex->perpix,

        .deb_obj = eat_debug_add_obj(
            tex,
            "texture",
            _ear_debug_texture_window,
            arena
            ),
        };

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
    eat_debug_remove_obj(tex->deb_obj);

    if (tex->hl_bindset != NULL) ear_delete_bindset(tex->hl_bindset);

    ear_vk_delete_texture(tex->vk);
    if (tex->stbi_pixels) stbi_image_free(tex->pixels);
    if (tex->self_pixels) free(tex->pixels);

    if (tex->dest != NULL) tex->dest->data = NULL;
    free(tex);
}

void
ear_resize_texture(
    ear_texture* tex,
    uint32_t width, uint32_t height
    ) {
    tex->width = width;
    tex->height = height;

    /*
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
    */
}


void
ear_get_texture_color(
    ear_texture* tex,
    uint32_t x, uint32_t y,
    float (*out)[4]
    ) {
    uint32_t tex_r = 2, tex_g = 1, tex_b = 0, tex_a = 3;
    if (tex->desc.type != EAR_TEX_COLOR) {
        tex_r = 0;
        tex_b = 2;
    }

    uint32_t i = (x + y * tex->width) * tex->perpix;
    (*out)[0] = (float)tex->pixels[i + tex_r] / 255.f;
    (*out)[1] = (float)tex->pixels[i + tex_g] / 255.f;
    (*out)[2] = (float)tex->pixels[i + tex_b] / 255.f;
    (*out)[3] = (float)tex->pixels[i + tex_a] / 255.f;
}

void
ear_set_texture_color(
    ear_texture* tex,
    uint32_t x, uint32_t y,
    float col[4]
    ) {
    uint32_t tex_r = 2, tex_g = 1, tex_b = 0, tex_a = 3;
    if (tex->desc.type != EAR_TEX_COLOR) {
        tex_r = 0;
        tex_b = 2;
    }

    uint32_t i = (x + y * tex->width) * tex->perpix;

    printf("%p,%p [%dx%d] (%d,%d), %d,%d, (%.3f,%.3f,%.3f,%.3f), %d, (%d,%d,%d,%d)\n", (void*)tex, tex->vk, tex->width,tex->height, tex->desc.type,tex->perpix, x,y, col[0], col[1], col[2], col[3], i, tex_r,tex_g,tex_b,tex_a);

    tex->pixels[i + tex_r] = col[0] * 255;
    tex->pixels[i + tex_g] = col[1] * 255;
    tex->pixels[i + tex_b] = col[2] * 255;
    tex->pixels[i + tex_a] = col[3] * 255;
}

void
ear_update_texture(
    ear_texture* tex
    ) {
    ear_vk_update_texture(tex->vk, tex->desc.type, tex->pixels, tex->width,tex->height);
}


void
_ear_debug_texture_window(
    void* _tex,
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
    (void)_tex;
    (void)x; (void)y; (void)w; (void)h;
    (void)t; (void)sel;

    /*
    ear_texture* tex = _tex;

    float offy = 0;
    float off = 16;
    
    char buf[64];

    snprintf(buf, sizeof(buf), "width: %d, height: %d", tex->width, tex->height);
    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    switch (tex->desc.type) {
    case EAR_TEX_COLOR: snprintf(buf, sizeof(buf), "type: color"); break;
    case EAR_TEX_DEPTH: snprintf(buf, sizeof(buf), "type: depth"); break;
    case EAR_TEX_HDR:   snprintf(buf, sizeof(buf), "type: hdr");   break;
    case EAR_TEX_HDR32: snprintf(buf, sizeof(buf), "type: hdr32"); break;
    }

    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    switch (tex->desc.filter) {
    case EAR_FILTER_NEAREST: snprintf(buf, sizeof(buf), "filter: nearest"); break;
    case EAR_FILTER_LINEAR:  snprintf(buf, sizeof(buf), "filter: linear");  break;
    }

    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    switch (tex->desc.wrap) {
    case EAR_WRAP_REPEAT: snprintf(buf, sizeof(buf), "wrap: repeat"); break;
    case EAR_WRAP_CLAMP:  snprintf(buf, sizeof(buf), "wrap: clamp"); break;
    case EAR_WRAP_COLOR:  snprintf(buf, sizeof(buf), "wrap: color ( rgba(%d, %d, %d, %d) )", 
        (int)(tex->desc.wrap_color[0] * 255), 
        (int)(tex->desc.wrap_color[1] * 255),
        (int)(tex->desc.wrap_color[2] * 255),
        (int)(tex->desc.wrap_color[3] * 255)
        ); break;
    }

    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    eau_rect rect = (eau_rect){ 0,0, tex->width, tex->height, EAU_ALIGN_TOP_LEFT };
    eau_rect fit = (eau_rect){ x, y+offy, w, h - offy, EAU_ALIGN_TOP_LEFT };

    rect = eau_rect_scale_to_fit(rect, fit);

    ear_rect(rect.x, rect.y, rect.w, rect.h, (float[4]){ 0,0,0,1 }, EAU_ALIGN_TOP_LEFT);
    ear_tex(tex, rect.x, rect.y, rect.w, rect.h, 0,0,tex->width,tex->height, (float[4]){ 1,1,1,1 }, EAU_ALIGN_TOP_LEFT);
    */
}
