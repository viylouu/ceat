#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../../framebuffer.h"

typedef struct ear_vk_framebuffer ear_vk_framebuffer;

ear_vk_framebuffer*
ear_vk_create_framebuffer(
    ear_framebuffer_desc desc
    );
void
ear_vk_delete_framebuffer(
    ear_vk_framebuffer* pln
    );

void
ear_vk_bind_framebuffer(
    ear_vk_framebuffer* pln
    );

struct ear_vk_framebuffer{
    VkRenderPass pass;
};
