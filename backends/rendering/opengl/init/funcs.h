#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define GLFUNC(name, ret, ...) \
    ret (APIENTRY *name)(__VA_ARGS__)
#else
#define GLFUNC(name, ret, ...) \
    ret (*name)(__VA_ARGS__)
#endif

#include <GL/gl.h>

#ifdef _WIN32
#include "../../include/GL/glext.h"
#else
#include <GL/glext.h>
#endif

typedef struct ear_gl{
    GLFUNC(createShader, GLuint, GLenum type);
    GLFUNC(deleteShader, void, GLuint shader);
    GLFUNC(shaderSource, void, GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
    GLFUNC(compileShader, void, GLuint shader);
    GLFUNC(getShaderIv, void, GLuint shader, GLenum pname, GLint* params);
    GLFUNC(getShaderInfoLog, void, GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog);
    
    GLFUNC(createProgram, GLuint, void);
    GLFUNC(deleteProgram, void, GLuint program);
    GLFUNC(attachShader, void, GLuint program, GLuint shader);
    GLFUNC(linkProgram, void, GLuint program);
    GLFUNC(getProgramIv, void, GLuint program, GLenum pname, GLint* params);
    GLFUNC(getProgramInfoLog, void, GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog);
    GLFUNC(useProgram, void, GLuint program);

    GLFUNC(genVertexArrays, void, GLsizei n, GLuint* arrays);
    GLFUNC(deleteVertexArrays, void, GLsizei n, const GLuint* arrays);
    GLFUNC(bindVertexArray, void, GLuint array);
    GLFUNC(vertexAttribFormat, void, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
    GLFUNC(vertexAttribIFormat, void, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
    GLFUNC(vertexAttribBinding, void, GLuint attribindex, GLuint bindingindex);
    GLFUNC(enableVertexAttribArray, void, GLuint index);

    GLFUNC(enable, void, GLenum cap);
    GLFUNC(disable, void, GLenum cap);

    GLFUNC(depthFunc, void, GLenum func);

    GLFUNC(cullFace, void, GLenum mode);
    GLFUNC(frontFace, void, GLenum mode);

    GLFUNC(blendFuncSeparate, void, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    GLFUNC(blendEquationSeparate, void, GLenum modeRGB, GLenum modeAlpha);

    GLFUNC(polygonMode, void, GLenum face, GLenum mode);

    GLFUNC(drawArraysInstanced, void, GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
    GLFUNC(drawElementsInstanced, void, GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);

    GLFUNC(clearColor, void, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    GLFUNC(clear, void, GLbitfield mask);

    GLFUNC(viewport, void, GLint x, GLint y, GLsizei width, GLsizei height);
    GLFUNC(scissor, void, GLint x, GLint y, GLsizei width, GLsizei height);

    GLFUNC(genBuffers, void, GLsizei n, GLuint* buffers);
    GLFUNC(deleteBuffers, void, GLsizei n, GLuint* buffers);
    GLFUNC(bindBuffer, void, GLenum target, GLuint buffer);
    GLFUNC(bufferData, void, GLenum target, GLsizeiptr size, const void* data, GLenum usage);
    GLFUNC(bindBufferBase, void, GLenum target, GLuint index, GLuint buffer);
    GLFUNC(bindVertexBuffer, void, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
    GLFUNC(bufferSubData, void, GLenum target, GLintptr offset, GLsizeiptr size, const void* data);

    GLFUNC(genTextures, void, GLsizei n, GLuint* textures);
    GLFUNC(deleteTextures, void, GLsizei n, const GLuint* textures);
    GLFUNC(bindTexture, void, GLenum target, GLuint texture);
    GLFUNC(texParameterI, void, GLenum target, GLenum pname, GLint param);
    GLFUNC(texParameterFv, void, GLenum target, GLenum pname, const GLfloat* params);
    GLFUNC(texImage2D, void, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
    GLFUNC(activeTexture, void, GLenum texture);
    GLFUNC(texSubImage2D, void, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
    GLFUNC(texImage3D, void, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
    GLFUNC(texSubImage3D, void, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels);

    GLFUNC(uniform1i, void, GLint location, GLint v0);

    GLFUNC(genFramebuffers, void, GLsizei n, GLuint* framebuffers);
    GLFUNC(deleteFramebuffers, void, GLsizei n, const GLuint* framebuffers);
    GLFUNC(bindFramebuffer, void, GLenum target, GLuint framebuffer);
    GLFUNC(framebufferTexture2D, void, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    GLFUNC(drawBuffer, void, GLenum mode);
    GLFUNC(drawBuffers, void, GLsizei n, const GLenum* bufs);
    GLFUNC(checkFramebufferStatus, GLenum, GLenum target);

    GLFUNC(bindBufferRange, void, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);

    GLFUNC(getIntegerV, void, GLenum pname, GLint* params);
} ear_gl;
extern ear_gl gl;

void
_ear_gl_get_funcs(
    void
    );
