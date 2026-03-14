#include "pipeline.h"
#include "../cutil.h"

#include "../eau/arena.h"
#include "gl.h"

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
    float x, float y, float w, float h
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


void
_ear_debug_pipeline_window(
    void* pln,
    float x, float y, float w, float h
    ) {

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
