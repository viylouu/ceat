#include "framebuffer.h"
//#include "../../cutil.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../eaw/window.h"
#include "../../eau/coll.h"
#include "../hl/data.h"
#include "../hl/user.h"

#include "rendering/impl.h"

#include "../hl/text.h"

ear_framebuffer* _ear_default_fb;
ear_framebuffer* _ear_master_fb;
ear_framebuffer* _ear_cur_framebuffer;

void
_ear_arena_framebuffer_delete(
    void* fb
    ) { 
    ear_delete_framebuffer(fb); 
}

void
_ear_debug_framebuffer_window(
    void* fb,
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

ear_framebuffer*
ear_create_framebuffer(
    ear_framebuffer_desc desc,
    eau_arena* arena
    ) {
    ear_framebuffer* fb = malloc(sizeof(ear_framebuffer));
    *fb = (ear_framebuffer){
        .desc = desc,

        .deb_obj = eat_debug_add_obj(
            fb,
            "framebuffer",
            _ear_debug_framebuffer_window,
            arena
            ),
        };

    if (desc.out_color_amt > 0) {
        fb->desc.out_colors = malloc(sizeof(ear_texture*) * desc.out_color_amt);
        memcpy(fb->desc.out_colors, desc.out_colors, sizeof(ear_texture*) * desc.out_color_amt);
    }

    fb->vk = ear_backend->framebuffer.create(&desc);

    if (arena != NULL) eau_add_to_arena(arena, &fb->dest, fb, _ear_arena_framebuffer_delete);
    return fb;
}

void
ear_delete_framebuffer(
    ear_framebuffer* fb
    ) {
    eat_debug_remove_obj(fb->deb_obj);

    ear_backend->framebuffer.delete(fb->vk);

    if (fb->desc.out_color_amt > 0) free(fb->desc.out_colors);

    if (fb->dest != NULL) fb->dest->data = NULL;
    free(fb);
}

void
ear_bind_framebuffer(
    ear_framebuffer* fb
    ) {
    ear_flush();

    if (fb != NULL) {
        ear_backend->framebuffer.bind(fb->vk);
        _ear_cur_framebuffer = fb;

        eau_mat4_ortho(&proj, 0,fb->desc.width, 0,fb->desc.height, 0,1);

        ear_backend->misc.viewport(0,0, fb->desc.width,fb->desc.height);
        ear_backend->misc.scissor (0,0, fb->desc.width,fb->desc.height);
    } else if (_ear_default_fb != NULL) {
        ear_bind_framebuffer(_ear_default_fb); 
    } else if (_ear_master_fb != NULL) {
        ear_bind_framebuffer(_ear_master_fb);
    } else {
        ear_backend->framebuffer.bind(NULL);
        _ear_cur_framebuffer = NULL;

        //if (vulkan or something)
        //  eau_mat4_ortho(&proj, 0, _eaw_window_width, 0, _eaw_window_height, 0,1);
        //else
            eau_mat4_ortho(&proj, 0, _eaw_window_width, _eaw_window_height, 0, 0,1);

        ear_backend->misc.viewport(0,0, _eaw_window_width,_eaw_window_height);
        ear_backend->misc.scissor (0,0, _eaw_window_width,_eaw_window_height);
    }
}

void
ear_resize_framebuffer(
    ear_framebuffer* fb,
    uint32_t width, uint32_t height
    ) {
    fb->desc.width = width;
    fb->desc.height = height;
}

void
ear_set_default_framebuffer(
    ear_framebuffer* fb
    ) {
    _ear_default_fb = fb;
}

void
_ear_set_master_framebuffer(
    ear_framebuffer* fb
    ) {
    _ear_master_fb = fb;
}


void
_ear_debug_framebuffer_window(
    void* _fb,
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* selected
    ) {
    (void)w; (void)h;

    ear_framebuffer* fb = _fb;

    float offy = 0;
    float off = 16;
    
    char buf[64];

    snprintf(buf, sizeof(buf), "width: %d, height: %d", fb->desc.width, fb->desc.height);
    ear_text(t.font, buf, x, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    for (uint32_t i = 0; i < fb->desc.out_color_amt; ++i) {
        uint32_t idx = 0;
        for (eat_debug_ll_obj* it = eat_debug_ll_first; it != NULL; it = it->next) {
            if (it->data == fb->desc.out_colors[i]) break;
            ++idx;
        }

        snprintf(buf, sizeof(buf), "color texture %d", idx);

        float width;
        ear_text_size(t.font, buf, 14, &width, NULL);

        bool sel = eau_point_rect(eaw_mouse_x,eaw_mouse_y, (eau_rect){ x,y+offy, width+8,16, EAU_ALIGN_TOP_LEFT });

        ear_rect(x,y+offy, width+8, 16, sel? debug_theme.sel_but_col : debug_theme.but_col, EAU_ALIGN_TOP_LEFT);
        ear_rect(x+2,y+2+offy, width+4, 12, sel? debug_theme.but_col : debug_theme.bg_col, EAU_ALIGN_TOP_LEFT);

        if (sel && eaw_is_mouse_pressed(EAW_MOUSE_LEFT)) *selected = idx;

        ear_text(t.font, buf, x + 4, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);

        offy += 18;
    }

    if (fb->desc.out_depth) {
        uint32_t idx = 0;
        for (eat_debug_ll_obj* it = eat_debug_ll_first; it != NULL; it = it->next) {
            if (it->data == fb->desc.out_depth) break;
            ++idx;
        }

        snprintf(buf, sizeof(buf), "depth texture %d", idx);

        float width;
        ear_text_size(t.font, buf, 14, &width, NULL);

        bool sel = eau_point_rect(eaw_mouse_x,eaw_mouse_y, (eau_rect){ x,y+offy, width+8,16, EAU_ALIGN_TOP_LEFT });

        ear_rect(x,y+offy, width+8, 16, sel? debug_theme.sel_but_col : debug_theme.but_col, EAU_ALIGN_TOP_LEFT);
        ear_rect(x+2,y+2+offy, width+4, 12, sel? debug_theme.but_col : debug_theme.bg_col, EAU_ALIGN_TOP_LEFT);

        if (sel && eaw_is_mouse_pressed(EAW_MOUSE_LEFT)) *selected = idx;

        ear_text(t.font, buf, x + 4, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);

        offy += 18;
    }
}
