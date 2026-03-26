#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../../eau/arena.h"
#include "../../debug/debug.h"

#include "buffer.h"

typedef struct ear_pipeline ear_pipeline;
typedef struct ear_pipeline_desc ear_pipeline_desc;

typedef struct ear_shader_desc ear_shader_desc;

typedef struct ear_vertex_attrib_desc ear_vertex_attrib_desc;

typedef enum ear_primitive_type{
    EAR_PRIM_FLOAT,
    EAR_PRIM_INT,
} ear_primitive_type;

typedef enum ear_cull_mode{
    EAR_CULL_NONE,
    EAR_CULL_FRONT,
    EAR_CULL_BACK,
} ear_cull_mode;
typedef enum ear_front_face{
    EAR_FRONT_CW,
    EAR_FRONT_CCW,
} ear_front_face;

typedef struct ear_blend_state ear_blend_state;
typedef enum ear_blend_factor{
    EAR_FAC_ZERO,
    EAR_FAC_ONE,
    EAR_FAC_SRC_COLOR,
    EAR_FAC_INV_SRC_COLOR,
    EAR_FAC_DST_COLOR,
    EAR_FAC_INV_DST_COLOR,
    EAR_FAC_SRC_ALPHA,
    EAR_FAC_INV_SRC_ALPHA,
    EAR_FAC_DST_ALPHA,
    EAR_FAC_INV_DST_ALPHA,
} ear_blend_factor;
typedef enum ear_blend_op{
    EAR_OP_ADD,
    EAR_OP_SUBTRACT,
    EAR_OP_REV_SUBTRACT,
    EAR_OP_MIN,
    EAR_OP_MAX,
} ear_blend_op;

typedef enum ear_fill_mode{
    EAR_FILL_FILL,
    EAR_FILL_LINE,
} ear_fill_mode;

typedef enum ear_topology{
    EAR_TOP_TRIS,
    EAR_TOP_LINES,
} ear_topology;

ear_pipeline*
ear_create_pipeline(
    ear_pipeline_desc desc,
    eau_arena* arena
    );

void
ear_delete_pipeline(
    ear_pipeline* pipeline
    );

void
ear_bind_pipeline(
    ear_pipeline* pipeline
    );


struct ear_shader_desc{
    const char* source;
    uint32_t source_size;
};

struct ear_vertex_attrib_desc{
    uint32_t location;
    ear_primitive_type type;
    uint32_t components;
    bool norm;
    uint32_t offset;
    uint32_t slot;
};

struct ear_blend_state{
    ear_blend_factor src_color;
    ear_blend_factor dst_color;
    ear_blend_op color_op;
    ear_blend_factor src_alpha;
    ear_blend_factor dst_alpha;
    ear_blend_op alpha_op;
};

struct ear_pipeline_desc{
    ear_shader_desc vertex;
    ear_shader_desc fragment;

    ear_buffer** vertex_buffers;
        uint32_t vertex_buffer_amt;
    ear_vertex_attrib_desc* vertex_attribs;
        uint32_t vertex_attrib_amt;

    ear_buffer_bind_set* bind_sets;
        uint32_t bind_set_amt;

    bool depth;
    bool depth_clamp;

    ear_cull_mode cull_mode;
    ear_front_face front_face;

    ear_blend_state blend_state;
        bool has_blend_state;

    ear_fill_mode fill_mode;

    ear_topology topology;
};

struct ear_pipeline{
    void* vk;

    ear_pipeline_desc desc;

    eau_destructor* dest;
    eat_debug_ll_obj* deb_obj;
};
