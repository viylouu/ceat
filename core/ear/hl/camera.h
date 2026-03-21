#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../eau/arena.h"
#include "../debug/debug.h"
#include "../eau/mat4.h"
#include "framebuffer.h"

typedef struct ear_camera ear_camera;
typedef struct ear_camera_desc ear_camera_desc;

typedef struct ear_camera_2d_desc ear_camera_2d_desc;
typedef struct ear_camera_3d_desc ear_camera_3d_desc;

//typedef enum ear_camera_type ear_camera_type;

extern ear_camera* ear_bound_camera;
extern bool ear_bound_camera_ui_mode;


ear_camera*
ear_create_camera(
    ear_camera_desc desc,
    eau_arena* arena
    );

void
ear_delete_camera(
    ear_camera* camera
    );

void
ear_bind_camera(
    ear_camera* camera,
    bool ui_mode
    );

void
ear_set_camera_position_2d(
    ear_camera* camera,
    float x, float y
    );

void
ear_set_camera_scale_2d(
    ear_camera* camera,
    float scalex, float scaley
    );

void
ear_set_camera_rotation_2d(
    ear_camera* camera,
    float rotation
    );


typedef enum ear_camera_type{
    EAR_CAMERA_2D,
    EAR_CAMERA_3D,
} ear_camera_type;

struct ear_camera_2d_desc{
    float x; float y;
    float scalex; float scaley;
    float rotation;
};
struct ear_camera_3d_desc{
    // float x; float y; float z;
    // float near; float far;
    // unimplemented due to lack of perspective matrix
};

struct ear_camera_desc{
    ear_framebuffer* fb;

    ear_camera_type type;
    union{
        ear_camera_2d_desc desc_2d;
        ear_camera_3d_desc desc_3d;
    };
};

struct ear_camera{
    mat4 matrix;

    ear_camera_desc desc;

    eau_destructor* dest;
    eat_debug_ll_obj* deb_obj;
};
