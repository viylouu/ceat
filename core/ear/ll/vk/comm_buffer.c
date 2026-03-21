#include "comm_buffer.h"
#include "../../../cutil.h"

#include "comm_pool.h"
#include "device_log.h"

VkCommandBuffer _ear_vk_comm_buffers[EAR_VK_MAX_FRAMES_IN_FLIGHT];

void
_ear_vk_create_command_buffers(
    void
    ) {
    VkCommandBufferAllocateInfo allocinfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,

        .commandPool = _ear_vk_comm_pool,

        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,

        .commandBufferCount = EAR_VK_MAX_FRAMES_IN_FLIGHT,
        };

    eat_assert(vkAllocateCommandBuffers(_ear_vk_device, &allocinfo, _ear_vk_comm_buffers) == VK_SUCCESS, "failed to create command buffers!");
}
