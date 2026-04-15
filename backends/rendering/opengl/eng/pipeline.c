#include "pipeline.h"
#include "cutil.h"

#include "../init/funcs.h"
#include "../util/pipeline.h"

ear_gl_pipeline* _ear_gl_cur_pln;

void*
ear_gl_create_pipeline(
    ear_pipeline_desc desc
    ) {
    ear_gl_pipeline* pln = malloc(sizeof(ear_gl_pipeline));
    pln->desc = desc;

    uint32_t vsh = gl.createShader(GL_VERTEX_SHADER);
    uint32_t fsh = gl.createShader(GL_FRAGMENT_SHADER);

    const char* vsh_sources[] = { pln->desc.vertex.source };
    gl.shaderSource(vsh, 1, vsh_sources, NULL);
    gl.compileShader(vsh);

    int32_t succ;
    gl.getShaderIv(vsh, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        printf("vertex shader comp failed!\n");
        char log[succ+1];
        gl.getShaderInfoLog(vsh, succ, NULL, log);
        log[succ] = '\0';
        printf("%s", log);

        eat_error("see error above!");
    }

    const char* fsh_sources[] = { pln->desc.fragment.source };
    gl.shaderSource(fsh, 1, fsh_sources, NULL);
    gl.compileShader(fsh);

    gl.getShaderIv(fsh, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        printf("fragment shader comp failed!\n");
        char log[succ+1];
        gl.getShaderInfoLog(fsh, succ, NULL, log);
        log[succ] = '\0';
        printf("%s", log);

        eat_error("see error above!");
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
        char log[succ+1];
        gl.getProgramInfoLog(pln->id, succ, NULL, log);
        log[succ] = '\0';
        printf("%s", log);

        eat_error("see error above!");
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

    return pln;
}
void
ear_gl_delete_pipeline(
    void* _pln
    ) {
    ear_gl_pipeline* pln = _pln;

    gl.deleteVertexArrays(1, &pln->vao);
    gl.deleteProgram(pln->id);

    free(pln);
}

void
ear_gl_bind_pipeline(
    void* _pln
    ) {
    ear_gl_pipeline* pln = _pln;

    _ear_gl_cur_pln = pln;

    gl.useProgram(pln->id);
    gl.bindVertexArray(pln->vao);

    if (pln->desc.depth) {
        gl.enable(GL_DEPTH_TEST);
        gl.depthFunc(GL_LESS);
    } else gl.disable(GL_DEPTH_TEST);

    if (pln->desc.cull_mode != EAR_CULL_NONE) {
        gl.enable(GL_CULL_FACE);
        gl.cullFace(_ear_gl_convert_cull_mode(pln->desc.cull_mode));
        gl.frontFace(_ear_gl_convert_front_face(pln->desc.front_face));
    } else gl.disable(GL_CULL_FACE);

    if (pln->desc.has_blend_state) {
        gl.enable(GL_BLEND);
        gl.blendFuncSeparate(
            _ear_gl_convert_blend_factor(pln->desc.blend_state.src_color),
            _ear_gl_convert_blend_factor(pln->desc.blend_state.dst_color),
            _ear_gl_convert_blend_factor(pln->desc.blend_state.src_alpha),
            _ear_gl_convert_blend_factor(pln->desc.blend_state.dst_alpha)
            );
        gl.blendEquationSeparate(
            _ear_gl_convert_blend_op(pln->desc.blend_state.color_op),
            _ear_gl_convert_blend_op(pln->desc.blend_state.alpha_op)
            );
    } else gl.disable(GL_BLEND);

    gl.polygonMode(GL_FRONT_AND_BACK, _ear_gl_convert_fill_mode(pln->desc.fill_mode));
}
