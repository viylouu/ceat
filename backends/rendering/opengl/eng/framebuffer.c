#include "framebuffer.h"
#include "cutil.h"

#include "../util/macros.h"
#include "../init/funcs.h"
#include "../gl.h"
#include "screen.h"
#include "texture.h"

ear_gl_framebuffer* _ear_gl_cur_fb = NULL;
uint32_t last_frame = 1792834;

void*
ear_gl_create_framebuffer(
    ear_framebuffer_desc* desc
    ) {
    ear_gl_framebuffer* fb = malloc(sizeof(ear_gl_framebuffer));
    fb->clear = desc->clear;
        fb->r = desc->clear_color[0];
        fb->g = desc->clear_color[1];
        fb->b = desc->clear_color[2];
        fb->a = desc->clear_color[3];

    gl.genFramebuffers(1, &fb->id);
    gl.bindFramebuffer(GL_FRAMEBUFFER, fb->id);

    uint32_t* dbufs = malloc(sizeof(uint32_t) * desc->out_color_amt);
    for (int i = 0; i < desc->out_color_amt; ++i) {
        gl.framebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D,
            get_gl(texture, desc->out_colors[i])->id,
            0
            );
        dbufs[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    if (desc->out_color_amt == 0) gl.drawBuffer(GL_NONE);
    else gl.drawBuffers(desc->out_color_amt, dbufs);

    if (desc->out_depth != NULL)
        gl.framebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            get_gl(texture, desc->out_depth)->id,
            0
            );

    eat_assert(gl.checkFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "incomplete framebuffer!");
    free(dbufs);

    return fb;
}
void
ear_gl_delete_framebuffer(
    void* _fb
    ) {
    ear_gl_framebuffer* fb = _fb;

    gl.deleteFramebuffers(1, &fb->id);

    free(fb);
}

void
ear_gl_bind_framebuffer(
    void* _fb
    ) {
    _ear_gl_cur_fb = _fb;

    if (_fb != NULL) {
        ear_gl_framebuffer* fb = _fb;
        gl.bindFramebuffer(GL_FRAMEBUFFER, fb->id);

        if (!fb->clear) return;

        if (fb->frame != _ear_gl_frame) _ear_gl_clear();
        fb->frame = _ear_gl_frame;
    } else {
        gl.bindFramebuffer(GL_FRAMEBUFFER, 0);

        if (last_frame != _ear_gl_frame) _ear_gl_clear();
        last_frame = _ear_gl_frame;
    }
}
