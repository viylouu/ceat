#include "texture.h"
#include "cutil.h"

GLenum 
_ear_gl_convert_texture_filter(
    ear_texture_filter filter
    ) {
    switch (filter) {
    case EAR_FILTER_NEAREST: return GL_NEAREST;
    case EAR_FILTER_LINEAR:  return GL_LINEAR;
    }

    eat_unreachable();
}
GLenum
_ear_gl_convert_texture_wrap(
    ear_texture_wrap wrap
    ) {
    switch (wrap) {
    case EAR_WRAP_REPEAT: return GL_REPEAT;
    case EAR_WRAP_CLAMP:  return GL_CLAMP_TO_EDGE;
    //case EAR_WRAP_COLOR:  return GL_CLAMP_TO_BORDER;
    case EAR_WRAP_MIRROR: return GL_MIRRORED_REPEAT;
    }

    eat_unreachable();
}
GLenum
_ear_gl_convert_texture_type_as_intf(
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
_ear_gl_convert_texture_type_as_f(
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
_ear_gl_convert_texture_type_as_type(
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
