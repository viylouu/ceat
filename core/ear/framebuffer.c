#include "framebuffer.h"
#include "../cutil.h"

#include "../eaw/window.h"
#include "../eau/mat4.h"
#include "data.h"
#include "user.h"
#include "gl.h"

ear_framebuffer* default_fb;

void
_ear_arena_framebuffer_delete(
    void* fb
    ) { 
    ear_delete_framebuffer(fb); 
}

ear_framebuffer*
ear_create_framebuffer(
    ear_framebuffer_desc desc,
    eau_arena* arena
    ) {
    ear_framebuffer* fb = malloc(sizeof(ear_framebuffer));
    *fb = (ear_framebuffer){
        .desc = desc,
        };

    gl.genFramebuffers(1, &fb->id);
    gl.bindFramebuffer(GL_FRAMEBUFFER, fb->id);

    uint32_t* dbufs = malloc(sizeof(uint32_t) * fb->desc.out_color_amt);
    for (int i = 0; i < fb->desc.out_color_amt; ++i) {
        gl.framebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D,
            fb->desc.out_colors[i]->id,
            0
            );
        dbufs[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    if (fb->desc.out_color_amt == 0) gl.drawBuffer(GL_NONE);
    else gl.drawBuffers(fb->desc.out_color_amt, dbufs);

    if (fb->desc.out_depth != NULL)
        gl.framebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            fb->desc.out_depth->id,
            0
            );

    eat_assert(gl.checkFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "incomplete framebuffer!");
    free(dbufs);

    gl.bindFramebuffer(GL_FRAMEBUFFER, 0);

    if (arena != NULL) eau_add_to_arena(arena, &fb->dest, fb, _ear_arena_framebuffer_delete);
    return fb;
}

void
ear_delete_framebuffer(
    ear_framebuffer* fb
    ) {
    gl.deleteFramebuffers(1, &fb->id);

    if (fb->dest != NULL) fb->dest->data = NULL;
    free(fb);
}

void
ear_bind_framebuffer(
    ear_framebuffer* fb
    ) {
    ear_flush();

    if (fb != NULL) {
        gl.bindFramebuffer(GL_FRAMEBUFFER, fb->id);
        eau_mat4_ortho(&proj, 0,fb->desc.width, 0,fb->desc.height, 0,1);
        gl.viewport(0,0, fb->desc.width, fb->desc.height);
    } else if (default_fb == NULL) {
        gl.bindFramebuffer(GL_FRAMEBUFFER, 0);
        eau_mat4_ortho(&proj, 0,eaw_window_width,eaw_window_height, 0, 0,1);
        gl.viewport(0,0, eaw_window_width,eaw_window_height);
    }
    else ear_bind_framebuffer(default_fb);
}

void
ear_set_default_framebuffer(
    ear_framebuffer* fb
    ) {
    default_fb = fb;
}
