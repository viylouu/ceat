#include "gl.h"

#include "eng/framebuffer.h"
#include "eng/texture.h"
#include "eng/pipeline.h"
#include "eng/bindset.h"
#include "init/funcs.h"
#include "eng/screen.h"
#include "eng/buffer.h"

eat_backend_rendering_impl ear_gl_impl = {
    .deps = {
        .opengl_context = true,
        .use_spirv = false,
        },

    .main = {
        .init = ear_gl_init,
        .exit = ear_gl_exit,
        .frame = ear_gl_frame,
        },
    .misc = {
        .clear = ear_gl_clear_color,

        .draw     = ear_gl_draw,
        .draw_idx = ear_gl_draw_idx,

        .scissor  = ear_gl_set_scissor,
        .viewport = ear_gl_set_viewport,
        },
    .framebuffer = {
        .create = ear_gl_create_framebuffer,
        .delete = ear_gl_delete_framebuffer,

        .bind = ear_gl_bind_framebuffer,
        },
    .texture = {
        .create = ear_gl_create_texture,
        .delete = ear_gl_delete_texture,

        .update = ear_gl_update_texture,
        },
    .pipeline = {
        .create = ear_gl_create_pipeline,
        .delete = ear_gl_delete_pipeline,

        .bind = ear_gl_bind_pipeline,
        },
    .bindset = {
        .create = ear_gl_create_bindset,
        .delete = ear_gl_delete_bindset,

        .update = ear_gl_update_bindset,

        .bind = ear_gl_bind_bindset,
        },
    .buffer = {
        .create = ear_gl_create_buffer,
        .delete = ear_gl_delete_buffer,

        .bind = ear_gl_bind_buffer,

        .update = ear_gl_update_buffer,
        },
    };

uint32_t _ear_gl_frame = 0;
int32_t _ear_gl_uni_align;

void
ear_gl_init(
    const char* title,
    int32_t width, int32_t height,
    bool vsync
    ) {
    _ear_gl_get_funcs();

    gl.getIntegerV(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &_ear_gl_uni_align);
}
void
ear_gl_exit(
    void
    ) {
}

void
ear_gl_frame(
    void
    ) {
    ++_ear_gl_frame;
}
