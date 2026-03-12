#include "camera.h"
#include "../cutil.h"

#include "../eaw/window.h"

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

ear_camera*
ear_create_camera(
    ear_camera_desc desc,
    eau_arena* arena
    ) {
    ear_camera* cam = malloc(sizeof(ear_camera));
    *cam = (ear_camera){
        .desc = desc,
        };

    _ear_camera_update(cam);

    if (arena != NULL) eau_add_to_arena(arena, &cam->dest, cam, _ear_arena_camera_delete);
    return cam;
}

void
ear_delete_camera(
    ear_camera* cam
    ) {
    if (cam->dest != NULL) cam->dest->data = NULL;
    free(cam);
}

void
ear_bind_camera(
    ear_camera* cam,
    bool ui_mode
    ) {
    ear_bind_framebuffer(cam->desc.fb);
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
