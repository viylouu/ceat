#pragma once

#include "ear/ll/pipeline.h"

typedef struct ear_gl_pipeline ear_gl_pipeline;

extern ear_gl_pipeline* _ear_gl_cur_pln;

void*
ear_gl_create_pipeline(
    ear_pipeline_desc desc
    );
void
ear_gl_delete_pipeline(
    void* pln
    );

void
ear_gl_bind_pipeline(
    void* pln
    );

struct ear_gl_pipeline{
    uint32_t id;
    uint32_t vao;

    ear_pipeline_desc desc;
};
