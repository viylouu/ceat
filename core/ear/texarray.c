#include "texarray.h"
#include "../cutil.h"

#include "../eau/arena.h"
#include "gl.h"

#include <string.h>

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
_ear_arena_texarray_delete(
    void* arr
    ) { 
    ear_delete_texarray(arr); 
}

ear_texarray*
ear_create_texarray(
    ear_texarray_desc desc,
    eau_arena* arena
    ) {
    ear_texarray* arr = malloc(sizeof(ear_texarray));
    *arr = (ear_texarray){
        .desc = desc,
        .texs = malloc(sizeof(ear_texture*) * desc.layers),
        };

    memset(arr->texs, 0, sizeof(ear_texture*) * desc.layers);

    gl.genTextures(1, &arr->id);
    eat_assert(arr->id != 0, "failed to create opengl texture!");

    gl.bindTexture(GL_TEXTURE_2D_ARRAY, arr->id);

    GLenum sampling = _TYPECONV_texture_filter(arr->desc.filter);
    GLenum wrap = _TYPECONV_texture_wrap(arr->desc.wrap);

    gl.texParameterI(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, sampling);
    gl.texParameterI(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, sampling);
    gl.texParameterI(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrap);
    gl.texParameterI(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrap);
    gl.texParameterFv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, arr->desc.wrap_color);

    gl.texImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,
        _TYPECONV_texture_type_as_intf(arr->desc.type),
        arr->desc.width,
        arr->desc.height,
        arr->desc.layers,
        0,
        _TYPECONV_texture_type_as_f(arr->desc.type),
        _TYPECONV_texture_type_as_type(arr->desc.type),
        NULL
        );

    gl.bindTexture(GL_TEXTURE_2D_ARRAY, 0);

    if (arena != NULL) eau_add_to_arena(arena, &arr->dest, arr, _ear_arena_texarray_delete);
    return arr;
}

void
ear_delete_texarray(
    ear_texarray* arr
    ) {
    gl.deleteTextures(1, &arr->id);
    free(arr->texs);

    if (arr->dest != NULL) arr->dest->data = NULL;
    free(arr);
}

void
ear_bind_texarray(
    ear_texarray* arr,
    uint32_t slot
    ) {
    gl.activeTexture(GL_TEXTURE0 + slot);
    gl.bindTexture(GL_TEXTURE_2D_ARRAY, arr->id);
    gl.uniform1i(slot, slot);
}

void
ear_add_to_texarray(
    ear_texarray* arr,
    ear_texture* tex,
    uint32_t layer
    ) {
    eat_assert(arr->desc.type == tex->desc.type, "texture type does not match texarray type!");
    eat_assert(layer < arr->desc.layers, "texture layer assignment out of range!");

    arr->texs[layer] = tex;

    gl.bindTexture(GL_TEXTURE_2D_ARRAY, arr->id);

    gl.texSubImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,
        0,0,
        layer,
        arr->desc.width,
        arr->desc.height,
        1,
        _TYPECONV_texture_type_as_f(arr->desc.type),
        _TYPECONV_texture_type_as_type(arr->desc.type),
        tex->pixels
        );

    gl.bindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void
ear_update_texarray(
    ear_texarray* arr
    ) {
    for (int i = 0; i < arr->desc.layers; ++i) {
        ear_texture* tex = arr->texs[i];
        if (tex == NULL) continue;
        ear_update_texarray_layer(arr, i);
    }
}

void
ear_update_texarray_layer(
    ear_texarray* arr,
    uint32_t layer
    ) {
    eat_assert(layer < arr->desc.layers, "texarray layer update out of range!");

    gl.bindTexture(GL_TEXTURE_2D_ARRAY, arr->id);

    gl.texSubImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,
        0,0,
        layer,
        arr->desc.width,
        arr->desc.height,
        1,
        _TYPECONV_texture_type_as_f(arr->desc.type),
        _TYPECONV_texture_type_as_type(arr->desc.type),
        arr->texs[layer]->pixels
        );

    gl.bindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
