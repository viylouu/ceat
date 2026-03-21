#include "gl.h"
#include "../cutil.h"

ear_gl gl;

#ifdef _WIN32
#define LOADGL(str_item, name) do { \
    str_item = (typeof(str_item))wglGetProcAddress(name); \
    eat_assert((str_item), "failed to load gl proc %s!", name); \
} while(0)
#else
#include <EGL/egl.h>
#include <GL/glx.h>

#define LOADGL(str_item, name) do { \
    if (_WALLYLAND) str_item = (typeof(str_item))eglGetProcAddress(name); \
    else str_item = (typeof(str_item))glXGetProcAddress((const GLubyte*)name); \
    eat_assert((str_item), "failed to load gl proc %s!", name); \
} while(0)
#endif

void
ear_gl_init(
    void
    ) {
#ifndef _WIN32
    const char* _WALLYLAND = getenv("WAYLAND_DISPLAY");
    const char* _X11 = getenv("DISPLAY");

    eat_assert(_WALLYLAND || _X11, "unknown display server!");
#endif

    LOADGL(gl.createShader, "glCreateShader");
    LOADGL(gl.deleteShader, "glDeleteShader");
    LOADGL(gl.shaderSource, "glShaderSource");
    LOADGL(gl.compileShader, "glCompileShader");
    LOADGL(gl.getShaderIv, "glGetShaderiv");
    LOADGL(gl.getShaderInfoLog, "glGetShaderInfoLog");

    LOADGL(gl.createProgram, "glCreateProgram");
    LOADGL(gl.deleteProgram, "glDeleteProgram");
    LOADGL(gl.attachShader, "glAttachShader");
    LOADGL(gl.linkProgram, "glLinkProgram");
    LOADGL(gl.getProgramIv, "glGetProgramiv");
    LOADGL(gl.getProgramInfoLog, "glGetProgramInfoLog");
    LOADGL(gl.useProgram, "glUseProgram");

    LOADGL(gl.genVertexArrays, "glGenVertexArrays");
    LOADGL(gl.deleteVertexArrays, "glDeleteVertexArrays");
    LOADGL(gl.bindVertexArray, "glBindVertexArray");
    LOADGL(gl.vertexAttribFormat, "glVertexAttribFormat");
    LOADGL(gl.vertexAttribIFormat, "glVertexAttribIFormat");
    LOADGL(gl.vertexAttribBinding, "glVertexAttribBinding");
    LOADGL(gl.enableVertexAttribArray, "glEnableVertexAttribArray");

    gl.enable = glEnable;
    gl.disable = glDisable;

    gl.depthFunc = glDepthFunc;

    gl.cullFace = glCullFace;
    gl.frontFace = glFrontFace;

    LOADGL(gl.blendFuncSeparate, "glBlendFuncSeparate");
    LOADGL(gl.blendEquationSeparate, "glBlendEquationSeparate");

    gl.polygonMode = glPolygonMode;
    
    LOADGL(gl.drawArraysInstanced, "glDrawArraysInstanced");

    gl.clearColor = glClearColor;
    gl.clear = glClear;

    gl.viewport = glViewport;
    gl.scissor = glScissor;

    LOADGL(gl.genBuffers, "glGenBuffers");
    LOADGL(gl.deleteBuffers, "glDeleteBuffers");
    LOADGL(gl.bindBuffer, "glBindBuffer");
    LOADGL(gl.bufferData, "glBufferData");
    LOADGL(gl.bindBufferBase, "glBindBufferBase");
    LOADGL(gl.bindVertexBuffer, "glBindVertexBuffer");
    LOADGL(gl.bufferSubData, "glBufferSubData");

    gl.genTextures = glGenTextures;
    gl.deleteTextures = glDeleteTextures;
    gl.bindTexture = glBindTexture;
    gl.texParameterI = glTexParameteri;
    gl.texParameterFv = glTexParameterfv;
    gl.texImage2D = glTexImage2D;
    gl.texSubImage2D = glTexSubImage2D;
    LOADGL(gl.activeTexture, "glActiveTexture");
    LOADGL(gl.texImage3D, "glTexImage3D");
    LOADGL(gl.texSubImage3D, "glTexSubImage3D");

    LOADGL(gl.uniform1i, "glUniform1i");

    LOADGL(gl.genFramebuffers, "glGenFramebuffers");
    LOADGL(gl.deleteFramebuffers, "glDeleteFramebuffers");
    LOADGL(gl.bindFramebuffer, "glBindFramebuffer");
    LOADGL(gl.framebufferTexture2D, "glFramebufferTexture2D");
    gl.drawBuffer = glDrawBuffer;
    LOADGL(gl.drawBuffers, "glDrawBuffers");
    LOADGL(gl.checkFramebufferStatus, "glCheckFramebufferStatus");
}
