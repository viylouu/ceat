#pragma once

#include "ear/ll/texture.h"

#include <GL/gl.h>

GLenum 
_ear_gl_convert_texture_filter(
    ear_texture_filter filter
    );
GLenum
_ear_gl_convert_texture_wrap(
    ear_texture_wrap wrap
    );
GLenum
_ear_gl_convert_texture_type_as_intf(
    ear_texture_type type
    );
GLenum
_ear_gl_convert_texture_type_as_f(
    ear_texture_type type
    );
GLenum
_ear_gl_convert_texture_type_as_type(
    ear_texture_type type
    );
