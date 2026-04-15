#include "screen.h"
#include "cutil.h"

GLenum
_ear_gl_convert_draw_mode(
    ear_topology mode
    ) {
    switch (mode) {
    case EAR_TOP_TRIS:  return GL_TRIANGLES;
    case EAR_TOP_LINES: return GL_LINES;
    }

    eat_unreachable();
}
