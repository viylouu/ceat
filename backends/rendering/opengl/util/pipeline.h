#pragma once

#include "ear/ll/pipeline.h"

#include <GL/gl.h>

GLenum
_ear_gl_convert_cull_mode(
    ear_cull_mode mode
    );
GLenum
_ear_gl_convert_front_face(
    ear_front_face face
    );
GLenum
_ear_gl_convert_blend_factor(
    ear_blend_factor fac
    );
GLenum
_ear_gl_convert_blend_op(
    ear_blend_op op
    );
GLenum
_ear_gl_convert_fill_mode(
    ear_fill_mode fill
    );
