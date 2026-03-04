#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef float mat4[16];

void 
eau_mat4_make(
    mat4* mat,
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33
    );

void 
eau_mat4_mult(
    mat4* out, 
    const mat4 a, 
    const mat4 b
    );

void 
eau_mat4_ortho(
    mat4* mat, 
    float left, float right,
    float bottom, float top,
    float near, float far
    );

void 
eau_mat4_identity(
    mat4* mat
    );

void 
eau_mat4_copy(
    mat4 from,
    mat4* to
    );

void 
eau_mat4_translate(
    mat4* mat, 
    float x, float y, float z
    );

void eau_mat4_scale(
    mat4* mat, 
    float x, float y, float z
    );

void eau_mat4_rotate_x(
    mat4* mat, 
    float ang
    );
void eau_mat4_rotate_y(
    mat4* mat, 
    float ang
    );
void eau_mat4_rotate_z(
    mat4* mat, 
    float ang
    );
