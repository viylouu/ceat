#pragma once

#include "../../core/ear/ll/bindset.h"
#include "../../core/ear/ll/buffer.h"
#include "../../core/ear/ll/framebuffer.h"
#include "../../core/ear/ll/pipeline.h"
#include "../../core/ear/ll/texture.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct eat_backend_rendering_impl{
    struct{
        void (*init)(
            const char* title,
            int32_t width, int32_t height,
            bool vsync
            );
        void (*exit)(
            void
            );

        void (*frame)(
            void
            );
    } main;
    struct{
        void (*draw)(
            uint32_t vertices,
            uint32_t instances
            );
        void (*draw_idx)(
            uint32_t indices,
            uint32_t instances
            );

        void (*viewport)(
            float x, float y,
            float w, float h
            );
        void (*scissor)(
            float x, float y,
            float w, float h
            );

        void (*clear)(
            void* fb,
            float r, float g, float b, float a
            );
    } misc;
    struct{
        void* (*create)(
            ear_pipeline_desc desc
            );
        void (*delete)(
            void* data
            );

        void (*bind)(
            void* data
            );
    } pipeline;
    struct{
        void* (*create)(
            ear_buffer_desc desc,
            void* data, uint32_t size
            );
        void (*delete)(
            void* data
            );

        void (*bind)(
            void* data,
            uint32_t slot,
            uint32_t offset
            );

        void (*update)(
            void* data,
            void* buf_data,
            uint32_t offset
            );
    } buffer;
    struct{
        void* (*create)(
            ear_bindset_desc desc
            );
        void (*delete)(
            void* data
            );

        void (*bind)(
            void* data,
            uint32_t slot,
            uint32_t offsets[],
            uint32_t offset_amt
            );
    } bindset;
    struct{
        void* (*create)(
            ear_framebuffer_desc desc
            );
        void (*delete)(
            void* data
            );

        void (*bind)(
            void* data
            );
    } framebuffer;
    struct{
        void* (*create)(
            ear_texture_desc desc,
            uint8_t** pixels,
            uint32_t width, uint32_t height
            );
        void (*delete)(
            void* data
            );

        void (*update)(
            void* data,
            uint8_t pixels[],
            uint32_t width,
            uint32_t height
            );
    } texture;
} eat_backend_rendering_impl;

extern eat_backend_rendering_impl* ear_backend;
