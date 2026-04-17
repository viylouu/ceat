#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

extern float _ear_clear_color[3];

void
ear_vk_set_viewport(
    float x, float y,
    float w, float h
    );
void
ear_vk_set_scissor(
    float x, float y,
    float w, float h
    );

void
ear_vk_draw(
    uint32_t vertices, uint32_t instances
    );
void
ear_vk_draw_idx(
    uint32_t indices, uint32_t instances
    );

void
ear_vk_clear(
    void* fb,
    float r, float g, float b, float a
    );
