#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../framebuffer.h"

typedef struct ear_vk_framebuffer ear_vk_framebuffer;

extern bool _ear_vk_is_first_fb;

ear_vk_framebuffer*
ear_vk_create_framebuffer(
    ear_framebuffer_desc desc
    );
void
ear_vk_delete_framebuffer(
    ear_vk_framebuffer* fb
    );

void
ear_vk_clear_framebuffer(
    ear_vk_framebuffer* fb,
    float r, float g, float b
    );

void
ear_vk_bind_framebuffer(
    ear_vk_framebuffer* fb
    );

struct ear_vk_framebuffer{
    VkRenderPass pass;
    VkFramebuffer fb;

    ear_framebuffer_desc desc;
};
