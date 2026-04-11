#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ear/ll/framebuffer.h"

typedef struct ear_vk_framebuffer ear_vk_framebuffer;

extern bool _ear_vk_is_first_fb;

void*
ear_vk_create_framebuffer(
    ear_framebuffer_desc desc
    );
void
ear_vk_delete_framebuffer(
    void* fb
    );

void
ear_vk_bind_framebuffer(
    void* fb
    );

struct ear_vk_framebuffer{
    //VkRenderPass pass;
    //VkFramebuffer fb;

    ear_framebuffer_desc desc;

    // used to do clears only on first bind
    uint32_t frame;
};
