#include "pipeline.h"
#include "cutil.h"

GLenum
_ear_gl_convert_cull_mode(
    ear_cull_mode mode
    ) {
    switch (mode) {
    case EAR_CULL_NONE:  return GL_NONE;
    case EAR_CULL_FRONT: return GL_FRONT;
    case EAR_CULL_BACK:  return GL_BACK;
    }

    eat_unreachable();
}
GLenum
_ear_gl_convert_front_face(
    ear_front_face face
    ) {
    switch (face) {
    case EAR_FRONT_CW:  return GL_CW;
    case EAR_FRONT_CCW: return GL_CCW;
    }

    eat_unreachable();
}
GLenum
_ear_gl_convert_blend_factor(
    ear_blend_factor fac
    ) {
    switch (fac) {
    case EAR_FAC_ZERO:          return GL_ZERO;
    case EAR_FAC_ONE:           return GL_ONE;
    case EAR_FAC_SRC_COLOR:     return GL_SRC_COLOR;
    case EAR_FAC_INV_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
    case EAR_FAC_DST_COLOR:     return GL_DST_COLOR;
    case EAR_FAC_INV_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
    case EAR_FAC_SRC_ALPHA:     return GL_SRC_ALPHA;
    case EAR_FAC_INV_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
    case EAR_FAC_DST_ALPHA:     return GL_DST_ALPHA;
    case EAR_FAC_INV_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
    }

    eat_unreachable();
}
GLenum
_ear_gl_convert_blend_op(
    ear_blend_op op
    ) {
    switch (op) {
    case EAR_OP_ADD:          return GL_FUNC_ADD;
    case EAR_OP_SUBTRACT:     return GL_FUNC_SUBTRACT;
    case EAR_OP_REV_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
    case EAR_OP_MIN:          return GL_MIN;
    case EAR_OP_MAX:          return GL_MAX;
    }

    eat_unreachable();
}
GLenum
_ear_gl_convert_fill_mode(
    ear_fill_mode fill
    ) {
    switch (fill) {
    case EAR_FILL_FILL: return GL_FILL;
    case EAR_FILL_LINE: return GL_LINE;
    }

    eat_unreachable();
}
