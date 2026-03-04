#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct eau_rect eau_rect;

typedef struct eau_collision_info eau_collision_info;


bool
eau_aabb2d(
    eau_rect a,
    eau_rect b
    );

bool
eau_point_aabb2d(
    float pointx, float pointy,
    eau_rect rect
    );

bool
eau_aabb3d(
    float min1x, float min1y, float min1z,
    float max1x, float max1y, float max1z,
    float min2x, float min2y, float min2z,
    float max2x, float max2y, float max2z
    );


bool
eau_gjk3d(
    float hull1[][3], uint32_t hull1_verts,
    float hull2[][3], uint32_t hull2_verts,
    float (*out_simplex)[4][3]
    );

eau_collision_info
eau_epa3d(
    float simplex[4][3],
    float hull1[][3], uint32_t hull1_verts,
    float hull2[][3], uint32_t hull2_verts
    );


struct eau_rect{
    float minx; float miny;
    float maxx; float maxy;
};

struct eau_collision_info{
    float normx; float normy; float normz;
    float depth;
};
