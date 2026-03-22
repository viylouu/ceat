#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../ll/pipeline.h"
#include "../ll/buffer.h"
//#include "../ll/texture.h"

#include "../../eau/mat4.h"

extern mat4 proj;
extern mat4 transf;

void
ear_user_init(
    void
    );

void 
ear_user_exit(
    void
    );


typedef struct ear_rect_rend{
    ear_pipeline* pln;
    ear_buffer* ubo;
        struct{ mat4 proj; } ubo_d;
    ear_buffer* ssbo;
        struct{ 
            float x; float y; 
            float w; float h; 
            float r; float g; float b; float a; 
            mat4 transf;
        } ssbo_d[4096];
        uint32_t ssbo_i;
} ear_rect_rend;
extern ear_rect_rend ear_rr;

void
ear_rect_rend_flush(
    void
    );


/*
typedef struct ear_tex_rend{
    ear_pipeline* pln;
    ear_buffer* ubo;
        struct{ mat4 proj; } ubo_d;
    ear_buffer* ssbo;
        struct{
            float x; float y;
            float w; float h;
            float r; float g; float b; float a;
            float sx; float sy; float sw; float sh;
            mat4 transf;
        } ssbo_d[4096];
        uint32_t ssbo_i;
    ear_texture* cur_tex;
} ear_tex_rend;
extern ear_tex_rend ear_tr;

void
ear_tex_rend_flush(
    void
    );
*/
