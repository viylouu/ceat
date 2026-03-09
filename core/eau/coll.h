#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct eau_rect eau_rect;

typedef struct eau_collision_info eau_collision_info;

//typedef enum eau_align eau_align;


bool
eau_aabb2d(
    float min1x, float min1y,
    float max1x, float max1y,
    float min2x, float min2y,
    float max2x, float max2y
    );

bool
eau_point_aabb2d(
    float pointx, float pointy,
    float minx, float miny,
    float maxx, float maxy
    );

bool
eau_rect_rect(
    eau_rect a,
    eau_rect b
    );

bool
eau_point_rect(
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


typedef enum eau_align{
    EAU_ALIGN_TOP_LEFT, EAU_ALIGN_TOP, EAU_ALIGN_TOP_RIGHT,
    EAU_ALIGN_MID_LEFT, EAU_ALIGN_MID, EAU_ALIGN_MID_RIGHT,
    EAU_ALIGN_BOT_LEFT, EAU_ALIGN_BOT, EAU_ALIGN_BOT_RIGHT,
} eau_align;

struct eau_rect{
    float x; float y;
    float w; float h;
    eau_align align;
};

struct eau_collision_info{
    float normx; float normy; float normz;
    float depth;
};


void
_CONV_topleftify(
    eau_rect* rect
    );
