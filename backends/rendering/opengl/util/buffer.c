#include "buffer.h"
#include "cutil.h"

GLenum
_ear_gl_convert_buffer_type(
    ear_buffer_type type
    ) {
    switch (type) {
    case EAR_BUF_UNIFORM: return GL_UNIFORM_BUFFER;
    case EAR_BUF_STORAGE: return GL_SHADER_STORAGE_BUFFER;
    case EAR_BUF_VERTEX:  return GL_ARRAY_BUFFER;
    case EAR_BUF_INDEX:   return GL_ELEMENT_ARRAY_BUFFER;
    }

    eat_unreachable();
}
