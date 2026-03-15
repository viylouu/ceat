#include "camera.h"
#include "../cutil.h"

#include "../eaw/window.h"
#include "../eau/coll.h"
#include "text.h"
#include "user.h"

ear_camera* ear_bound_camera = NULL;
bool ear_bound_camera_ui_mode = false;

void
_ear_camera_update(
    ear_camera* cam
    );

void
_ear_arena_camera_delete(
    void* cam
    ) { 
    ear_delete_camera(cam); 
}

void
_ear_debug_camera_window(
    void* cam,
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

ear_camera*
ear_create_camera(
    ear_camera_desc desc,
    eau_arena* arena
    ) {
    ear_camera* cam = malloc(sizeof(ear_camera));
    *cam = (ear_camera){
        .desc = desc,

        .deb_obj = eat_debug_add_obj(
            cam,
            "camera",
            _ear_debug_camera_window
            ),
        };

    _ear_camera_update(cam);

    if (arena != NULL) eau_add_to_arena(arena, &cam->dest, cam, _ear_arena_camera_delete);
    return cam;
}

void
ear_delete_camera(
    ear_camera* cam
    ) {
    eat_debug_remove_obj(cam->deb_obj);

    if (cam->dest != NULL) cam->dest->data = NULL;
    free(cam);
}

void
ear_bind_camera(
    ear_camera* cam,
    bool ui_mode
    ) {
    if (ear_bound_camera != cam) ear_bind_framebuffer(cam->desc.fb);
    ear_bound_camera_ui_mode = ui_mode;
    ear_bound_camera = cam;
}

void
ear_set_camera_position_2d(
    ear_camera* cam,
    float x, float y
    ) {
    eat_assert(cam->desc.type == EAR_CAMERA_2D, "camera type must be 2d to apply a 2d transformation");
    cam->desc.desc_2d.x = x;
    cam->desc.desc_2d.y = y;
    _ear_camera_update(cam);
}

void
ear_set_camera_scale_2d(
    ear_camera* cam,
    float scalex, float scaley
    ) {
    eat_assert(cam->desc.type == EAR_CAMERA_2D, "camera type must be 2d to apply a 2d transformation");
    cam->desc.desc_2d.scalex = scalex;
    cam->desc.desc_2d.scaley = scaley;
    _ear_camera_update(cam);
}

void
ear_set_camera_rotation_2d(
    ear_camera* cam,
    float rot
    ) {
    eat_assert(cam->desc.type == EAR_CAMERA_2D, "camera type must be 2d to apply a 2d transformation");
    cam->desc.desc_2d.rotation = rot;
    _ear_camera_update(cam);
}



void
_ear_camera_update(
    ear_camera* cam
    ) {
    switch (cam->desc.type) {
    case EAR_CAMERA_2D:
        float centerx; float centery;
        if (cam->desc.fb) {
            centerx = cam->desc.fb->desc.width * .5;
            centery = cam->desc.fb->desc.height * .5;
        } else {
            centerx = eaw_window_width * .5;
            centery = eaw_window_height * .5;
        }

        mat4 o;
        
        eau_mat4_translate(&cam->matrix, -cam->desc.desc_2d.x, -cam->desc.desc_2d.y, 0);
        eau_mat4_rotate_z(&o, cam->desc.desc_2d.rotation);
        eau_mat4_mult(&cam->matrix, cam->matrix, o);
        eau_mat4_scale(&o, cam->desc.desc_2d.scalex, cam->desc.desc_2d.scaley, 1);
        eau_mat4_mult(&cam->matrix, cam->matrix, o);
        eau_mat4_translate(&o, centerx, centery, 0);
        eau_mat4_mult(&cam->matrix, cam->matrix, o);

        break;
    case EAR_CAMERA_3D: eat_assert(false, "3d camera type is not yet supported!");
    }
}


void
_ear_debug_camera_window(
    void* _cam,
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* selected
    ) {
    ear_camera* cam = _cam;

    float offy = 0;
    float off = 16;
    
    char buf[64];

    uint32_t idx = 0;
    for (eat_debug_ll_obj* it = eat_debug_ll_first; it != NULL; it = it->next) {
        if (it->data == cam->desc.fb) break;
        ++idx;
    }

    snprintf(buf, sizeof(buf), "framebuffer %d", idx);

    float width;
    ear_text_size(t.font, buf, 14, &width, NULL);

    bool sel = eau_point_rect(eaw_mouse_x,eaw_mouse_y, (eau_rect){ x,y+offy, width+8,16 });

    ear_rect(x,y+offy, width+8, 16, sel? debug_theme.sel_but_col : debug_theme.but_col, EAU_ALIGN_TOP_LEFT);
    ear_rect(x+2,y+2+offy, width+4, 12, sel? debug_theme.but_col : debug_theme.bg_col, EAU_ALIGN_TOP_LEFT);

    if (sel && eaw_is_mouse_pressed(EAW_MOUSE_LEFT)) *selected = idx;

    ear_text(t.font, buf, x + 4, y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += 18;

    switch (cam->desc.type) {
    case EAR_CAMERA_2D: snprintf(buf, sizeof(buf), "type: 2d"); break;
    case EAR_CAMERA_3D: snprintf(buf, sizeof(buf), "type: 3d"); break;
    }

    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    switch (cam->desc.type) {
    case EAR_CAMERA_2D:
        snprintf(buf, sizeof(buf), "position: %.3f, %.3f", cam->desc.desc_2d.x, cam->desc.desc_2d.y);
        ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
        offy += off;

        snprintf(buf, sizeof(buf), "scale: %.3f, %.3f", cam->desc.desc_2d.scalex, cam->desc.desc_2d.scaley);
        ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
        offy += off;

        snprintf(buf, sizeof(buf), "rotation: %.3f", cam->desc.desc_2d.rotation);
        ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
        offy += off;

        break;
    case EAR_CAMERA_3D:
        break;
    }
}
