#include "ear.h"
//#include "../cutil.h"

#include "data.h"
#include "gl.h"

#include <GL/gl.h>
#include <GL/glext.h>

void
ear_init(
    void
    ) {
    ear_gl_init();
    ear_user_init();
}

void 
ear_stop(
    void
    ) {
    ear_user_stop();
}

void
ear_frame(
    void
    ) {
    ear_set_default_framebuffer(NULL);
    ear_bind_framebuffer(NULL);
}
