#include "pipeline.h"
#include "../../cutil.h"

//#include "../eau/arena.h"
//#include "text.h"

#include "vk/eng/pipeline.h"

void
_ear_arena_pipeline_delete(
    void* pln
    ) { 
    ear_delete_pipeline(pln); 
}

void
_ear_debug_pipeline_window(
    void* pln,
    float x, float y, float w, float h,
    eat_debug_theme theme,
    int32_t* sel
    );

ear_pipeline*
ear_create_pipeline(
    ear_pipeline_desc desc,
    eau_arena* arena
    ) {
    ear_pipeline* pln = malloc(sizeof(ear_pipeline));
    *pln = (ear_pipeline){
        .vk = ear_vk_create_pipeline(desc),

        .desc = desc,

        .deb_obj = eat_debug_add_obj(
            pln,
            "pipeline",
            _ear_debug_pipeline_window,
            arena
            ),
        };

    if (arena != NULL) eau_add_to_arena(arena, &pln->dest, pln, _ear_arena_pipeline_delete);
    return pln;
}

void
ear_delete_pipeline(
    ear_pipeline* pln
    ) {
    eat_debug_remove_obj(pln->deb_obj);

    ear_vk_delete_pipeline(pln->vk);

    if (pln->dest != NULL) pln->dest->data = NULL;
    free(pln);
}

void
ear_bind_pipeline(
    ear_pipeline* pln
    ) {
    ear_vk_bind_pipeline(pln->vk);
}


char*
_TYPECONV_blend_factor_to_string(
    char buf[64],
    ear_blend_factor fac
    ) {
    switch (fac) {
    case EAR_FAC_ZERO: snprintf(buf, 64, "zero"); break;
    case EAR_FAC_ONE: snprintf(buf, 64, "one"); break;
    case EAR_FAC_SRC_COLOR: snprintf(buf, 64, "src-color"); break;
    case EAR_FAC_DST_COLOR: snprintf(buf, 64, "dst-color"); break;
    case EAR_FAC_INV_SRC_COLOR: snprintf(buf, 64, "inv-src-color"); break;
    case EAR_FAC_INV_DST_COLOR: snprintf(buf, 64, "inv-dst-color"); break;
    case EAR_FAC_SRC_ALPHA: snprintf(buf, 64, "src-alpha"); break;
    case EAR_FAC_DST_ALPHA: snprintf(buf, 64, "dst-alpha"); break;
    case EAR_FAC_INV_SRC_ALPHA: snprintf(buf, 64, "inv-src-alpha"); break;
    case EAR_FAC_INV_DST_ALPHA: snprintf(buf, 64, "inv-dst-alpha"); break;
    }

    return buf;
}

char*
_TYPECONV_blend_op_to_string(
    char buf[64],
    ear_blend_op op
    ) {
    switch (op) {
    case EAR_OP_ADD: snprintf(buf, 64, "add"); break;
    case EAR_OP_SUBTRACT: snprintf(buf, 64, "subtract"); break;
    case EAR_OP_REV_SUBTRACT: snprintf(buf, 64, "rev-subtract"); break;
    case EAR_OP_MIN: snprintf(buf, 64, "min"); break;
    case EAR_OP_MAX: snprintf(buf, 64, "max"); break;
    }

    return buf;
}

void
_ear_debug_pipeline_window(
    void* _pln,
    float x, float y, float w, float h,
    eat_debug_theme t,
    int32_t* sel
    ) {
    /*ear_pipeline* pln = _pln;

    float offy = 0;
    float off = 16;
    
    char buf[64];

    snprintf(buf, sizeof(buf), "depth: %s", pln->desc.depth? "enabled" : "disabled");
    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    switch (pln->desc.cull_mode) {
    case EAR_CULL_NONE: snprintf(buf, sizeof(buf), "culling: none"); break;
    case EAR_CULL_FRONT: snprintf(buf, sizeof(buf), "culling: front"); break;
    case EAR_CULL_BACK: snprintf(buf, sizeof(buf), "culling: back"); break;
    }

    ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
    offy += off;

    if (pln->desc.cull_mode != EAR_CULL_NONE) {
        switch (pln->desc.front_face) {
        case EAR_FRONT_CW: snprintf(buf, sizeof(buf), "front: cw"); break;
        case EAR_FRONT_CCW: snprintf(buf, sizeof(buf), "front: ccw"); break;
        }

        ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
        offy += off;
    }

    if (pln->desc.blend_state.has_state) {
        ear_text(t.font, "blend state:", x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
        offy += off;

        char buf2[64];

#define blend_txt(name, name_act, type) do { \
    snprintf(buf, sizeof(buf), "- %s: %s", name, _TYPECONV_blend_##type##_to_string(buf2, pln->desc.blend_state.state.name_act)); \
        ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT); \
        offy += off; \
} while(0)

        blend_txt("src-color", src_color, factor);
        blend_txt("dst-color", dst_color, factor);
        blend_txt("color-op", color_op, op);
        blend_txt("src-alpha", src_alpha, factor);
        blend_txt("dst-alpha", dst_alpha, factor);
        blend_txt("alpha-op", alpha_op, op);
    } else {
        ear_text(t.font, "blending disabled", x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
        offy += off;
    }

    if (pln->desc.vertex_attrib_amt == 0) {
        ear_text(t.font, "no vertex attribs", x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
        offy += off;
    } else {
        ear_text(t.font, "vertex attribs:", x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
        offy += off;

        for (uint32_t i = 0; i < pln->desc.vertex_attrib_amt; ++i) {
            ear_vertex_attrib_desc attrib = pln->desc.vertex_attribs[i];

            char type_buf[64];
            switch (attrib.type) {
            case EAR_PRIM_FLOAT: snprintf(type_buf, sizeof(type_buf), "float"); break;
            case EAR_PRIM_INT: snprintf(type_buf, sizeof(type_buf), "int"); break;
            }

            snprintf(buf, sizeof(buf), "- slot: %d, loc: %d, type: %s, comps: %d, norm: %s, off: %d", 
                attrib.slot,
                attrib.location,
                type_buf,
                attrib.components,
                attrib.norm? "true" : "false",
                attrib.offset
                );

            ear_text(t.font, buf, x,y+offy, 14, t.text_col, EAU_ALIGN_TOP_LEFT);
            offy += off;
        }
    }
    */
}
