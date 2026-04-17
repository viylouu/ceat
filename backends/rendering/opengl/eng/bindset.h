#pragma once

#include "ear/ll/bindset.h"
typedef struct ear_gl_bindset ear_gl_bindset;

void*
ear_gl_create_bindset(
    ear_bindset_desc desc
    );
void
ear_gl_delete_bindset(
    void* set
    );

void
ear_gl_update_bindset(
    void* set,
    ear_bindset_desc desc
    );

void
ear_gl_bind_bindset(
    void* set,
    uint32_t slot,
    const uint32_t* offsets,
    uint32_t offset_amt
    );

struct ear_gl_bindset{
    ear_bindset_desc desc;
};
