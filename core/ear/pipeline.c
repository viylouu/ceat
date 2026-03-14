#include "pipeline.h"
#include "../cutil.h"

#include "../eau/arena.h"
#include "gl.h"
#include "text.h"

GLenum
_TYPECONV_cull_mode(
    ear_cull_mode mode
    );

GLenum
_TYPECONV_front_face(
    ear_front_face face
    );

GLenum
_TYPECONV_blend_factor(
    ear_blend_factor fac
    );

GLenum
_TYPECONV_blend_op(
    ear_blend_op op
    );

GLenum
_TYPECONV_fill_mode(
    ear_fill_mode fill
    );


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
        .desc = desc,

        .deb_obj = eat_debug_add_obj(
            pln,
            "pipeline",
            _ear_debug_pipeline_window
            ),
        };

    uint32_t vsh = gl.createShader(GL_VERTEX_SHADER);
    uint32_t fsh = gl.createShader(GL_FRAGMENT_SHADER);

    const char* vsh_sources[] = { pln->desc.vertex.source };
    gl.shaderSource(vsh, 1, vsh_sources, NULL);
    gl.compileShader(vsh);

    int32_t succ;
    gl.getShaderIv(vsh, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        printf("vertex shader comp failed!\n");
        char log[1024];
        gl.getShaderInfoLog(vsh, 1024, NULL, log);
        printf("%s", log);

        eat_assert(false, "see error above!");
    }

    const char* fsh_sources[] = { pln->desc.fragment.source };
    gl.shaderSource(fsh, 1, fsh_sources, NULL);
    gl.compileShader(fsh);

    gl.getShaderIv(fsh, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        printf("fragment shader comp failed!\n");
        char log[1024];
        gl.getShaderInfoLog(fsh, 1024, NULL, log);
        printf("%s", log);

        eat_assert(false, "see error above!");
    }

    pln->id = gl.createProgram();
    gl.attachShader(pln->id, vsh);
    gl.attachShader(pln->id, fsh);
    gl.linkProgram(pln->id);

    gl.deleteShader(fsh);
    gl.deleteShader(vsh);

    gl.getProgramIv(pln->id, GL_LINK_STATUS, &succ);
    if (!succ) {
        printf("pipeline comp failed!\n");
        char log[1024];
        gl.getProgramInfoLog(fsh, 1024, NULL, log);
        printf("%s", log);

        eat_assert(false, "see error above!");
    }

    gl.genVertexArrays(1, &pln->vao);
    gl.bindVertexArray(pln->vao);

    for (int i = 0; i < pln->desc.vertex_attrib_amt; ++i) {
        ear_vertex_attrib_desc attrib = pln->desc.vertex_attribs[i];
        switch (attrib.type) {
        case EAR_PRIM_FLOAT: 
            gl.vertexAttribFormat(
                attrib.location,
                attrib.components,
                GL_FLOAT,
                attrib.norm? GL_TRUE : GL_FALSE,
                attrib.offset
                );
            break;
        case EAR_PRIM_INT: 
            gl.vertexAttribIFormat(
                attrib.location,
                attrib.components,
                GL_INT,
                attrib.offset
                );
            break;
        }

        gl.vertexAttribBinding(
            attrib.location,
            attrib.slot
            );
        gl.enableVertexAttribArray(attrib.location);
    }

    gl.bindVertexArray(0);

    if (arena != NULL) eau_add_to_arena(arena, &pln->dest, pln, _ear_arena_pipeline_delete);
    return pln;
}

void
ear_delete_pipeline(
    ear_pipeline* pln
    ) {
    eat_debug_remove_obj(pln->deb_obj);

    gl.deleteVertexArrays(1, &pln->vao);
    gl.deleteProgram(pln->id);

    if (pln->dest != NULL) pln->dest->data = NULL;
    free(pln);
}

void
ear_bind_pipeline(
    ear_pipeline* pln
    ) {
    gl.useProgram(pln->id);
    gl.bindVertexArray(pln->vao);

    if (pln->desc.depth) {
        gl.enable(GL_DEPTH_TEST);
        gl.depthFunc(GL_LESS);
    } else gl.disable(GL_DEPTH_TEST);

    if (pln->desc.cull_mode != EAR_CULL_NONE) {
        gl.enable(GL_CULL_FACE);
        gl.cullFace(_TYPECONV_cull_mode(pln->desc.cull_mode));
        gl.frontFace(_TYPECONV_front_face(pln->desc.front_face));
    } else gl.disable(GL_CULL_FACE);

    if (pln->desc.blend_state.has_state) {
        gl.enable(GL_BLEND);
        gl.blendFuncSeparate(
            _TYPECONV_blend_factor(pln->desc.blend_state.state.src_color),
            _TYPECONV_blend_factor(pln->desc.blend_state.state.dst_color),
            _TYPECONV_blend_factor(pln->desc.blend_state.state.src_alpha),
            _TYPECONV_blend_factor(pln->desc.blend_state.state.dst_alpha)
            );
        gl.blendEquationSeparate(
            _TYPECONV_blend_op(pln->desc.blend_state.state.color_op),
            _TYPECONV_blend_op(pln->desc.blend_state.state.alpha_op)
            );
    } else gl.disable(GL_BLEND);

    gl.polygonMode(GL_FRONT_AND_BACK, _TYPECONV_fill_mode(pln->desc.fill_mode));
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
    ear_pipeline* pln = _pln;

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
}



GLenum
_TYPECONV_cull_mode(
    ear_cull_mode mode
    ) {
    switch (mode) {
    case EAR_CULL_NONE:  return GL_NONE;
    case EAR_CULL_FRONT: return GL_FRONT;
    case EAR_CULL_BACK:  return GL_BACK;
    }

    eat_unreachable();
}

GLenum
_TYPECONV_front_face(
    ear_front_face face
    ) {
    switch (face) {
    case EAR_FRONT_CW:  return GL_CW;
    case EAR_FRONT_CCW: return GL_CCW;
    }

    eat_unreachable();
}

GLenum
_TYPECONV_blend_factor(
    ear_blend_factor fac
    ) {
    switch (fac) {
    case EAR_FAC_ZERO:          return GL_ZERO;
    case EAR_FAC_ONE:           return GL_ONE;
    case EAR_FAC_SRC_COLOR:     return GL_SRC_COLOR;
    case EAR_FAC_INV_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
    case EAR_FAC_DST_COLOR:     return GL_DST_COLOR;
    case EAR_FAC_INV_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
    case EAR_FAC_SRC_ALPHA:     return GL_SRC_ALPHA;
    case EAR_FAC_INV_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
    case EAR_FAC_DST_ALPHA:     return GL_DST_ALPHA;
    case EAR_FAC_INV_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
    }

    eat_unreachable();
}

GLenum
_TYPECONV_blend_op(
    ear_blend_op op
    ) {
    switch (op) {
    case EAR_OP_ADD:          return GL_FUNC_ADD;
    case EAR_OP_SUBTRACT:     return GL_FUNC_SUBTRACT;
    case EAR_OP_REV_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
    case EAR_OP_MIN:          return GL_MIN;
    case EAR_OP_MAX:          return GL_MAX;
    }

    eat_unreachable();
}

GLenum
_TYPECONV_fill_mode(
    ear_fill_mode fill
    ) {
    switch (fill) {
    case EAR_FILL_FILL: return GL_FILL;
    case EAR_FILL_LINE: return GL_LINE;
    }

    eat_unreachable();
}
