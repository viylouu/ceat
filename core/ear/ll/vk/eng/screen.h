#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

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
ear_vk_clear(
    float r, float g, float b, float a
    );
