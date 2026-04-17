#include "texture.h"
#include "cutil.h"

#include "../init/funcs.h"
#include "../util/texture.h"

void*
ear_gl_create_texture(
    ear_texture_desc desc,
    uint8_t** pixels,
    uint32_t width, uint32_t height
    ) {
    ear_gl_texture* tex = malloc(sizeof(ear_gl_texture));
    tex->desc = desc;

    gl.genTextures(1, &tex->id);
    eat_assert(tex->id != 0, "failed to create opengl texture!");

    gl.bindTexture(GL_TEXTURE_2D, tex->id);

    GLenum sampling = _ear_gl_convert_texture_filter(tex->desc.filter);
    GLenum wrap = _ear_gl_convert_texture_wrap(tex->desc.wrap);

    gl.texParameterI(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampling);
    gl.texParameterI(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampling);
    gl.texParameterI(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    gl.texParameterI(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    //gl.texParameterFv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, tex->desc.wrap_color);

    gl.texImage2D(
        GL_TEXTURE_2D,
        0,
        _ear_gl_convert_texture_type_as_intf(tex->desc.type),
        width, height,
        0,
        _ear_gl_convert_texture_type_as_f(tex->desc.type),
        _ear_gl_convert_texture_type_as_type(tex->desc.type),
        *pixels
        );

    return tex;
}
void
ear_gl_delete_texture(
    void* _tex
    ) {
    ear_gl_texture* tex = _tex;

    gl.deleteTextures(1, &tex->id);

    free(tex);
}

void
ear_gl_update_texture(
    void* _tex,
    const uint8_t* pixels,
    uint32_t width, uint32_t height
    ) {
    ear_gl_texture* tex = _tex;

    gl.bindTexture(GL_TEXTURE_2D, tex->id);

    if (width != tex->width || height != tex->height) {
        tex->width = width; tex->height = height;

        gl.texImage2D(
            GL_TEXTURE_2D,
            0,
            _ear_gl_convert_texture_type_as_intf(tex->desc.type),
            width, height,
            0,
            _ear_gl_convert_texture_type_as_f(tex->desc.type),
            _ear_gl_convert_texture_type_as_type(tex->desc.type),
            pixels
            );
    } else {
        gl.texSubImage2D(
            GL_TEXTURE_2D,
            0,
            0,0,
            tex->width, tex->height,
            _ear_gl_convert_texture_type_as_f(tex->desc.type),
            _ear_gl_convert_texture_type_as_type(tex->desc.type),
            pixels
            );
    }
}
